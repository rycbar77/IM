#ifndef RECVFILETHREAD_H
#define RECVFILETHREAD_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include "msg.h"
class recvFileThread : public QObject
{
    Q_OBJECT
public:
public:
    recvFileThread(int sockfd,QObject *parent = 0);
    ~recvFileThread();

    void start();
    static void* run(void* arg);
private:
    int sockfd;
    pthread_t thread;
};


#endif // RECVFILETHREAD_H
