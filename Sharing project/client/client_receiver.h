#pragma once
#include <QThread>
#include <QString>
#include "shm.h"

class ClientReceiver : public QThread {
    Q_OBJECT

public:
    ClientReceiver(ShmData* shm);
    void stop();

signals:
    void messageReceived(QString msg);

protected:
    void run() override;

private:
    ShmData* shm;
    unsigned long long lastSeq = 0;
    bool running = true;
};

