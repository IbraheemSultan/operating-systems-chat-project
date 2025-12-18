#include "server_window.h"
#include "server_receiver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

ServerWindow::ServerWindow(QWidget* parent) : QMainWindow(parent) {
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

    // ===== Shared Memory =====
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(ShmData));
    shm = (ShmData*)mmap(nullptr, sizeof(ShmData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pthread_mutexattr_t mattr;
    pthread_condattr_t cattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);

    shm->seq = 0;
    pthread_mutex_init(&shm->mutex, &mattr);
    pthread_cond_init(&shm->cond, &cattr);

    // ===== Receiver Thread =====
    receiver = new ServerReceiver(shm);
    connect(receiver, &ServerReceiver::messageReceived, this, &ServerWindow::appendMessage);
    receiver->start();

    connect(sendButton, &QPushButton::clicked, this, &ServerWindow::sendMessage);
}

ServerWindow::~ServerWindow() {
    receiver->stop();
    receiver->wait();

    munmap(shm, sizeof(ShmData));
    shm_unlink(SHM_NAME);
}

void ServerWindow::sendMessage() {
    std::string input = inputLine->text().toStdString();
    if (input.empty()) return;

    pthread_mutex_lock(&shm->mutex);

    shm->seq++;
    unsigned index = shm->seq % MAX_MESSAGES;

    std::string msg = "Server: " + input;
    strncpy(shm->msgs[index], msg.c_str(), MSG_SIZE-1);
    shm->msgs[index][MSG_SIZE-1] = '\0';

    pthread_cond_broadcast(&shm->cond);
    pthread_mutex_unlock(&shm->mutex);

    inputLine->clear();
}

void ServerWindow::appendMessage(QString msg) {
    chatView->append(msg);
}

