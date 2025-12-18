#pragma once
#include <string>
#include <functional>
#include <QTcpSocket>
#include <QObject>

class ClientCore : public QObject
{
    Q_OBJECT
public:
    ClientCore(QObject *parent = nullptr);
    ~ClientCore();

    void connectToServer(const std::string &ip, int port, const std::string &username);
    void disconnectFromServer();
    void sendMessage(const std::string &msg);
    bool isConnected() const;

    std::function<void(const std::string&)> logCallback;
    std::function<void(const std::string&)> messageCallback;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    std::string username;
};

