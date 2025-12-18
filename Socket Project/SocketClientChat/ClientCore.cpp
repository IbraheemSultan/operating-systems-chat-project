#include "ClientCore.h"
#include <QHostAddress>
#include <QString>

ClientCore::ClientCore(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ClientCore::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientCore::onDisconnected);
}

ClientCore::~ClientCore() {
    disconnectFromServer();
}

void ClientCore::connectToServer(const std::string &ip, int port, const std::string &uname) {
    username = uname;
    socket->connectToHost(QString::fromStdString(ip), port);

    if(socket->waitForConnected(3000)) {
        std::string nameMsg = "/name " + username;
        socket->write(nameMsg.c_str(), nameMsg.size());

        if(logCallback) logCallback("✔ Connected as " + username);
    } else {
        if(logCallback) logCallback("❌ Failed to connect to server");
    }
}

void ClientCore::disconnectFromServer() {
    if(socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void ClientCore::sendMessage(const std::string &msg) {
    if(socket->state() == QAbstractSocket::ConnectedState && !msg.empty())
        socket->write(msg.c_str(), msg.size());
}

bool ClientCore::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void ClientCore::onReadyRead() {
    QByteArray data = socket->readAll();
    if(messageCallback)
        messageCallback(std::string(data.constData(), data.size()));
}

void ClientCore::onDisconnected() {
    if(logCallback)
        logCallback("⚠ Disconnected from server");
}

