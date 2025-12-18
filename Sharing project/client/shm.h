#pragma once
#include <pthread.h>

#define SHM_NAME "/my_chat_memory"
#define MAX_MESSAGES 32
#define MSG_SIZE 256

struct ShmData {
    unsigned long long seq;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    char msgs[MAX_MESSAGES][MSG_SIZE];
};

