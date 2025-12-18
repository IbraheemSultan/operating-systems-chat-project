#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include "ServerCore.h"

class server : public QMainWindow
{
    Q_OBJECT
public:
    explicit server(QWidget *parent = nullptr);
    ~server();

private slots:
    void startServer();
    void stopServer();

private:
    void log(QString msg);

private:
    QLineEdit *portEdit;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QTextEdit *logBox;

    ServerCore* core;   // pointer to the backend server
};

