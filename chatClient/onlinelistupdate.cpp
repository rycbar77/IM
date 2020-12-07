#include "onlinelistupdate.h"
#include <unistd.h>
onlineListUpdate::onlineListUpdate(int sockfd,int my_id,QObject *parent) :
    sockfd(sockfd),my_id(my_id),QObject(parent)
{

}

void onlineListUpdate::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << Qt::endl;
}

void* onlineListUpdate::run(void* arg)
{
    onlineListUpdate* th = (onlineListUpdate*)(arg);
    Msg msg;
    while(1)
    {
        msg.to_user_id=-1;
        msg.from_user_id = th->my_id;
        int ret = send(th->sockfd,&msg,sizeof(msg),0);
        if(-1 == ret || 0 == ret)
            break;
        emit th->listupdate();
        sleep(5);
    }
    return nullptr;
}
