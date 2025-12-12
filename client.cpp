#include "client.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

client::client(QWidget *parent)
    : QMainWindow(parent)
{
    socket = new QTcpSocket(this);

    // --- UI --- //
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Top connection bar
    QHBoxLayout *top = new QHBoxLayout();
    ipEdit = new QLineEdit("127.0.0.1");
    portEdit = new QLineEdit("5000");
    portEdit->setValidator(new QIntValidator(1, 65535));

    connectBtn = new QPushButton("Connect");
    disconnectBtn = new QPushButton("Disconnect");
    disconnectBtn->setEnabled(false);

    top->addWidget(ipEdit);
    top->addWidget(portEdit);
    top->addWidget(connectBtn);
    top->addWidget(disconnectBtn);

    // Chat box
    chatBox = new QTextEdit();
    chatBox->setReadOnly(true);

    // Bottom send bar
    QHBoxLayout *bottom = new QHBoxLayout();
    msgEdit = new QLineEdit();
    sendBtn = new QPushButton("Send");
    sendBtn->setEnabled(false);

    bottom->addWidget(msgEdit);
    bottom->addWidget(sendBtn);

    mainLayout->addLayout(top);
    mainLayout->addWidget(chatBox);
    mainLayout->addLayout(bottom);

    setWindowTitle("Chat Client");
    resize(500, 400);

    // Connect signals
    connect(connectBtn, &QPushButton::clicked, this, &client::connectToServer);
    connect(disconnectBtn, &QPushButton::clicked, this, &client::disconnectFromServer);
    connect(sendBtn, &QPushButton::clicked, this, &client::sendMessage);

    connect(socket, &QTcpSocket::readyRead, this, &client::receiveMessage);
}

client::~client() {}

void client::log(QString msg)
{
    chatBox->append(msg);
}

void client::connectToServer()
{
    QString ip = ipEdit->text();
    int port = portEdit->text().toInt();

    socket->connectToHost(ip, port);

    if (!socket->waitForConnected(2000)) {
        log("❌ Failed to connect to server.");
        return;
    }

    log("✔ Connected to server.");

    connectBtn->setEnabled(false);
    disconnectBtn->setEnabled(true);
    sendBtn->setEnabled(true);
}

void client::disconnectFromServer()
{
    socket->disconnectFromHost();
    log("⚠ Disconnected from server.");

    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
    sendBtn->setEnabled(false);
}

void client::sendMessage()
{
    QString msg = msgEdit->text().trimmed();
    if (msg.isEmpty()) return;

    socket->write(msg.toUtf8());
    log("You: " + msg);
    msgEdit->clear();
}

void client::receiveMessage()
{
    QString msg = socket->readAll();
    log("Server: " + msg);
}
