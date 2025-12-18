#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "shm.h"

class ClientReceiver;

class ClientWindow : public QMainWindow {
    Q_OBJECT

public:
    ClientWindow(QWidget* parent = nullptr);
    ~ClientWindow();

private slots:
    void sendMessage();
    void appendMessage(QString msg);

private:
    ShmData* shm;
    ClientReceiver* receiver;

    QTextEdit* chatView;
    QLineEdit* inputLine;
    QPushButton* sendButton;
    QString username;
};

