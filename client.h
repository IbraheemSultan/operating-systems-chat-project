#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

private slots:
    void connectToServer();
    void disconnectFromServer();
    void sendMessage();
    void receiveMessage();

private:
    QTcpSocket *socket;

    // GUI elements
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QTextEdit *chatBox;
    QLineEdit *msgEdit;
    QPushButton *sendBtn;

    void log(QString msg);
};

#endif // CLIENT_H
