#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QListWidget;

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private:
    // UI Elements
    QLabel *usernameLabel;
    QLabel *serverIPLabel;
    QLabel *portLabel;
    QLabel *chatLabel;
    QLabel *usersLabel;
    QLabel *messageLabel;

    QLineEdit *usernameEdit;
    QLineEdit *serverIPEdit;
    QLineEdit *portEdit;
    QLineEdit *messageEdit;

    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *sendButton;

    QTextEdit *chatText;


    // Core
    class ClientCore* core;
};

#endif // CLIENT_H

