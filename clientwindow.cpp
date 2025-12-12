#include "ClientWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Socket Chat - Client");
    setFixedSize(600, 400);

    // Create UI Elements
    usernameLabel = new QLabel("Username:");
    serverIPLabel = new QLabel("Server IP:");
    portLabel = new QLabel("Port:");
    chatLabel = new QLabel("Chat");
    usersLabel = new QLabel("Users");
    messageLabel = new QLabel("Message:");

    usernameEdit = new QLineEdit();
    serverIPEdit = new QLineEdit();
    portEdit = new QLineEdit();
    messageEdit = new QLineEdit();

    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");
    sendButton = new QPushButton("Send");

    chatText = new QTextEdit();
    chatText->setReadOnly(true);
    usersList = new QListWidget();

    socket = new QTcpSocket(this);

    // Layout setup
    QGridLayout *topLayout = new QGridLayout();
    topLayout->addWidget(usernameLabel, 0, 0);
    topLayout->addWidget(usernameEdit, 0, 1, 1, 2);
    topLayout->addWidget(connectButton, 0, 3);
    topLayout->addWidget(disconnectButton, 0, 4);

    topLayout->addWidget(serverIPLabel, 1, 0);
    topLayout->addWidget(serverIPEdit, 1, 1);
    topLayout->addWidget(portLabel, 1, 2);
    topLayout->addWidget(portEdit, 1, 3);

    QHBoxLayout *chatLayout = new QHBoxLayout();
    QVBoxLayout *chatBoxLayout = new QVBoxLayout();
    chatBoxLayout->addWidget(chatLabel);
    chatBoxLayout->addWidget(chatText);

    QVBoxLayout *usersBoxLayout = new QVBoxLayout();
    usersBoxLayout->addWidget(usersLabel);
    usersBoxLayout->addWidget(usersList);

    chatLayout->addLayout(chatBoxLayout, 3);
    chatLayout->addLayout(usersBoxLayout, 1);

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(messageEdit);
    messageLayout->addWidget(sendButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(chatLayout);
    mainLayout->addLayout(messageLayout);

    setLayout(mainLayout);

    // Connections
    connect(connectButton, &QPushButton::clicked, this, &ClientWindow::connectToServer);
    connect(disconnectButton, &QPushButton::clicked, this, &ClientWindow::disconnectFromServer);
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::readMessage);
}

ClientWindow::~ClientWindow() {
    socket->disconnectFromHost();
}

// Slots
void ClientWindow::connectToServer() {
    QString ip = serverIPEdit->text();
    quint16 port = portEdit->text().toUShort();
    if(ip.isEmpty() || port == 0) {
        QMessageBox::warning(this, "Error", "Please enter valid IP and Port!");
        return;
    }
    socket->connectToHost(ip, port);
}

void ClientWindow::disconnectFromServer() {
    socket->disconnectFromHost();
}

void ClientWindow::sendMessage() {
    QString msg = messageEdit->text();
    if(!msg.isEmpty() && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(msg.toUtf8());
        messageEdit->clear();
    }
}

void ClientWindow::readMessage() {
    QByteArray data = socket->readAll();
    chatText->append(QString::fromUtf8(data));
}
