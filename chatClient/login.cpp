#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    sockfd = -1;
    sockfd2 = -1;
    sockfd3 = -1;
    ui->setupUi(this);
    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(loginTo()));
    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
}

void Login::loginTo()
{
    if(sockfd != -1)
        cs.closeSockfd(sockfd);

    if(sockfd2 != -1)
        cs2.closeSockfd(sockfd2);

    if(sockfd2 != -1)
        cs3.closeSockfd(sockfd3);

    sockfd = cs.connectTo("49.232.4.79",9999);
    if(sockfd == -1)
        return;

    sockfd2 = cs2.connectTo("49.232.4.79",9998);
    if(sockfd2 == -1)
        return;

    sockfd3 = cs.connectTo("49.232.4.79",9997);
    if(sockfd3 == -1)
        return;

    loginInfo lf;
    lf.flag = 1;
    strcpy(lf.id ,ui->lineEdit_account->text().toStdString().c_str());
    strcpy(lf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());
    if(!strcmp(lf.id,"") || !strcmp(lf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << Qt::endl;
        return;
    }
    ssize_t size = send(sockfd,(void*)&lf,sizeof(lf),0);
    if(-1 == size)
    {
        return;
    }
    int flag = 0;
    size = recv(sockfd,&flag,sizeof(int),0);
    if(1 == flag)
    {
        qDebug() << "login successful" << Qt::endl;
        main_face = new interface(sockfd,sockfd2,sockfd3,atoi(lf.id));
        main_face->show();
        this->close();
        //delete this;
    }
    else
    {
        return;
    }
}

void Login::registerTo()
{
    if(-1 == sockfd)
    {
        connectServer cs;
        sockfd = cs.connectTo("49.232.4.79",9999);
        if(-1 == sockfd)
        {
            qDebug() << "register don't connect server" << Qt::endl;
            return;
        }
    }
    regist = new Register(sockfd);
    regist->show();
}
