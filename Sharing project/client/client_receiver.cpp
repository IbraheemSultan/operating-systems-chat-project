#include "client_receiver.h"
#include <pthread.h>

ClientReceiver::ClientReceiver(ShmData* s) {
    shm = s;
}

void ClientReceiver::stop() {
    running = false;
}

void ClientReceiver::run() {
    while (running) {
        pthread_mutex_lock(&shm->mutex);

        while (lastSeq == shm->seq) {
            pthread_cond_wait(&shm->cond, &shm->mutex);
        }

        while (lastSeq < shm->seq) {
            lastSeq++;
            unsigned index = lastSeq % MAX_MESSAGES;
            emit messageReceived(QString::fromUtf8(shm->msgs[index]));
        }

        pthread_mutex_unlock(&shm->mutex);
    }
}

