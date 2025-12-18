#include "client_window.h"
#include "client_receiver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

ClientWindow::ClientWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    chatView = new QTextEdit(this);
    chatView->setReadOnly(true);
    layout->addWidget(chatView);

    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLine = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    inputLayout->addWidget(inputLine);
    inputLayout->addWidget(sendButton);
    layout->addLayout(inputLayout);

    // ===== Username =====
    bool ok;
    username = QInputDialog::getText(this, "Username", "Enter username:", QLineEdit::Normal, "Client", &ok);
    if (!ok || username.isEmpty()) username = "Client";

    // ===== Open Shared Memory =====
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        chatView->setText("Shared memory not found. Start server first.");
        sendButton->setEnabled(false);
        inputLine->setEnabled(false);
        return;
    }

    shm = (ShmData*)mmap(nullptr, sizeof(ShmData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // ===== Receiver Thread =====
    receiver = new ClientReceiver(shm);
    connect(receiver, &ClientReceiver::messageReceived, this, &ClientWindow::appendMessage);
    receiver->start();

    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
}

ClientWindow::~ClientWindow() {
    if (receiver) {
        receiver->stop();
        receiver->wait();
    }
    if (shm) {
        munmap(shm, sizeof(ShmData));
    }
}

void ClientWindow::sendMessage() {
    QString input = inputLine->text();
    if (input.isEmpty()) return;

    pthread_mutex_lock(&shm->mutex);

    shm->seq++;
    unsigned index = shm->seq % MAX_MESSAGES;

    QString msg = username + ": " + input;
    QByteArray ba = msg.toUtf8();
    strncpy(shm->msgs[index], ba.constData(), MSG_SIZE-1);
    shm->msgs[index][MSG_SIZE-1] = '\0';

    pthread_cond_broadcast(&shm->cond);
    pthread_mutex_unlock(&shm->mutex);

    inputLine->clear();
}

void ClientWindow::appendMessage(QString msg) {
    chatView->append(msg);
}

