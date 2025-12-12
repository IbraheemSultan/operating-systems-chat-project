#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QTcpSocket>

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QListWidget;

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void connectToServer();
    void disconnectFromServer();
    void sendMessage();
    void readMessage();

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
    QListWidget *usersList;

    // Socket
    QTcpSocket *socket;
};

#endif // CLIENTWINDOW_H
