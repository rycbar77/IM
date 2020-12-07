#include "file.h"
#include "ui_file.h"
#include <cstdio>
#include <QMessageBox>
#include <sys/stat.h>
file::file(int sockfd,int my_id,int to_id,QString filename,QWidget *parent) :
    sockfd(sockfd),my_id(my_id),to_id(to_id),filename(filename),QWidget(parent),
    ui(new Ui::file)
{
    msg.from_user_id=my_id;
    msg.to_user_id=to_id;
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

void file::fileTransfer()
{
    char  buffer[4096];
    FILE *fp;



    bzero(buffer,sizeof(buffer));
    strcpy(msg.msg,filename.toStdString().c_str());
    send(sockfd, &msg, sizeof(msg),0);
    //    struct stat buf;
    //    stat(filename.toStdString().c_str(),&buf);
    int fileSize=get_file_size(filename.toStdString().c_str());
    qDebug()<<fileSize<<Qt::endl;
    if(fileSize>0){
        if( ( fp = fopen(filename.toStdString().c_str(),"rb") ) == NULL ){
            QMessageBox::information(nullptr, QString::fromLocal8Bit("选择的文件"), QString::fromLocal8Bit("没有选择文件"));
            return;
        }
        int len=0;
        send(sockfd, &fileSize, sizeof(fileSize),0);
        bzero(buffer,sizeof(buffer));
        int sum=0;
        ui->progressBar->setRange(0,fileSize);
        while(!feof(fp)){
            len = fread(buffer, 1, sizeof(buffer), fp);
            //        QString qStr(buffer);
            //        qDebug()<<qStr<<Qt::endl;
            //        memcpy(msg.msg,buffer,len);
            sum+=len;
            ui->progressBar->setValue(sum);
            if(len != send(sockfd, buffer, len,0)){
                qDebug()<<"write."<<Qt::endl;
                break;
            }
        }
        //    qDebug()<<sum<<Qt::endl;
        fclose(fp);
        QMessageBox msgBox;
        msgBox.setText("文件发送成功！！！");

        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok){
            this->hide();
        }
    }
}

file::~file()
{
    delete ui;
}
