#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "shm.h"

class ServerReceiver;

class ServerWindow : public QMainWindow {
    Q_OBJECT

public:
    ServerWindow(QWidget* parent = nullptr);
    ~ServerWindow();

private slots:
    void sendMessage();
    void appendMessage(QString msg);

private:
    ShmData* shm;
    ServerReceiver* receiver;

    QTextEdit* chatView;
    QLineEdit* inputLine;
    QPushButton* sendButton;
};

