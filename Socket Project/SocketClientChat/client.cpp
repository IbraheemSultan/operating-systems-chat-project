#include "Client.h"
#include "ClientCore.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

Client::Client(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Chat Client GUI");
    setFixedSize(600, 400);

    // Labels
    usernameLabel = new QLabel("Username:");
    serverIPLabel = new QLabel("Server IP:");
    portLabel = new QLabel("Port:");
    chatLabel = new QLabel("Chat");
    usersLabel = new QLabel("Users");
    messageLabel = new QLabel("Message:");

    // LineEdits
    usernameEdit = new QLineEdit();
    serverIPEdit = new QLineEdit();
    portEdit = new QLineEdit();
    messageEdit = new QLineEdit();

    // Buttons
    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");
    sendButton = new QPushButton("Send");

    // Chat & Users
    chatText = new QTextEdit();
    chatText->setReadOnly(true);
    

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


    chatLayout->addLayout(chatBoxLayout, 3);
    

    QHBoxLayout *messageLayout = new QHBoxLayout();
    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(messageEdit);
    messageLayout->addWidget(sendButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(chatLayout);
    mainLayout->addLayout(messageLayout);

    setLayout(mainLayout);

    // Core setup
    core = new ClientCore(this);
    core->logCallback = [&](const std::string &msg){
        chatText->append(QString::fromStdString(msg));
    };
    core->messageCallback = [&](const std::string &msg){
        chatText->append(QString::fromStdString(msg));
    };

    // Connections
    connect(connectButton, &QPushButton::clicked, [=](){
        core->connectToServer(serverIPEdit->text().toStdString(),
                              portEdit->text().toInt(),
                              usernameEdit->text().toStdString());
    });

    connect(disconnectButton, &QPushButton::clicked, [=](){
        core->disconnectFromServer();
    });

    connect(sendButton, &QPushButton::clicked, [=](){
        core->sendMessage(messageEdit->text().toStdString());
        messageEdit->clear();
    });
}

Client::~Client() {
    core->disconnectFromServer();
}

