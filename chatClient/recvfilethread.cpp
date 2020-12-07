#include "recvfilethread.h"
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

recvFileThread::recvFileThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvFileThread::~recvFileThread()
{

}

void recvFileThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << Qt::endl;
}

void* recvFileThread::run(void* arg)
{
    FILE *fp;
    int  n;
    recvFileThread* th = (recvFileThread*)(arg);
    char buffer[4096];
    Msg msg;
    while(1)
    {
        //        int ret = recv(th->sockfd,&buffer,sizeof(buffer),0);
        //        if(-1 == ret || 0 == ret)
        //            break;

        int ret=recv(th->sockfd,&msg,sizeof(msg),0);
        //        qDebug()<<msg.msg<<Qt::endl;
        std::string filename(msg.msg);
        if(filename.find_last_of('/')!=-1)
            filename=filename.substr(filename.find_last_of('/'));
        std::string path="./files/"+filename;
        if(opendir("./files")==NULL)
        {
            mkdir("./files",0755);
        }


        int filesize=0;
        ret=recv(th->sockfd,&filesize,sizeof(filesize),0);
        qDebug()<<filesize<<Qt::endl;
        if(filesize>0)
        {
            if((fp = fopen(path.c_str(),"wb") ) == NULL )
            {

            }

            int sum=0;
            while(1){
                bzero(buffer,sizeof(buffer));
                n = recv(th->sockfd,buffer,sizeof (buffer),0);
                //            qDebug()<<n<<Qt::endl;
                sum+=n;
                //            QString qStr(buffer);
                //            qDebug()<<qStr<<Qt::endl;
                //            qDebug()<<"recving!!!!"<<Qt::endl;

                fwrite(buffer, 1, n, fp);
                if(sum>=filesize)
                    break;
            }

            buffer[n] = '\0';
            fclose(fp);
        }
    }
    return nullptr;
}
