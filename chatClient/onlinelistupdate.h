#ifndef ONLINELISTUPDATE_H
#define ONLINELISTUPDATE_H
#include "msg.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <QObject>

class onlineListUpdate : public QObject
{
    Q_OBJECT
public:
    onlineListUpdate(int sockfd,int my_id,QObject *parent = 0);
    ~onlineListUpdate()=default;

    void start();
    static void* run(void* arg);
signals:
    void listupdate();
private:
    int sockfd;
    int my_id;
    pthread_t thread;
};

#endif // ONLINELISTUPDATE_H
