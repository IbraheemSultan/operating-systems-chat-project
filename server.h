#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

class server : public QMainWindow
{
    Q_OBJECT

public:
    server(QWidget *parent = nullptr);
    ~server();

private slots:
    void startServer();
    void stopServer();
    void newConnection();
    void readClient();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;

    // GUI elements
    QTextEdit *logBox;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QLineEdit *portEdit;

    void log(QString msg);
};

#endif // SERVER_H
