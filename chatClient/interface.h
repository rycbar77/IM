#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <iostream>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include "connectserver.h"
#include "msg.h"
#include "recvmsgthread.h"
#include "onlinelistupdate.h"
#include "recvfilethread.h"

using namespace std;

struct online_userinfo{
    char id[20]={0};
    char name[20]={0};
};

namespace Ui {
class interface;
}

class interface : public QWidget
{
    Q_OBJECT

public:
    explicit interface(int sockfd,int sockfd2,int sockfd3,int my_id,QWidget *parent = 0);
    ~interface();

protected:
    void closeEvent(QCloseEvent * event);
private:
    Ui::interface *ui;

    int sockfd;
    int sockfd2;
    int sockfd3;

    Msg msg;

    int chatType;

    int my_id;
    int to_user_id;
    QString to_user_name;
    QString fileName;

    recvMsgThread* thread;
    onlineListUpdate* thread2;
    recvFileThread* thread3;

    void recv_Online_userinfo();

    void delete_Msg();
    void filetransfer(QString filename);

private slots:
    void getItem();
    void btn_one_slot();
    void btn_all_slot();
    void btn_send_slot();
    void update_Online_userinfo();
    void selectfile();
    void recv_msg(Msg *msg);
};

#endif // INTERFACE_H
