#include "interface.h"
#include "ui_interface.h"
#include "file.h"
#include "onlinelistupdate.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cstdio>

interface::interface(int sockfd,int sockfd2,int sockfd3, int my_id, QWidget *parent) :
    sockfd(sockfd),sockfd2(sockfd2),sockfd3(sockfd3),my_id(my_id),QWidget(parent),
    ui(new Ui::interface)
{
    ui->setupUi(this);
    chatType = 1;
    to_user_id = 0;
    msg.to_user_id = 0;
    msg.from_user_id = this->my_id;
    //msg.from_user_name[0]=0;
    ui->btn_one->setEnabled(false);

    ui->tableWidget_userlist->setColumnCount(2);
    QStringList list;
    list << "userID" << "name";
    ui->tableWidget_userlist->setHorizontalHeaderLabels(list);
    ui->tableWidget_userlist->verticalHeader()->setVisible(false);
    ui->tableWidget_userlist->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_userlist->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableWidget_msgPage->setColumnCount(1);
    ui->tableWidget_msgPage->setShowGrid(false);
    ui->tableWidget_msgPage->setColumnWidth(0,500);
    ui->tableWidget_msgPage->verticalHeader()->setVisible(false);
    ui->tableWidget_msgPage->horizontalHeader()->setVisible(false);
    ui->tableWidget_msgPage->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget_msgPage->scrollToTop();

    recv_Online_userinfo();

    thread = new recvMsgThread(sockfd);
    thread->start();

    thread2=new onlineListUpdate(sockfd,my_id);
    thread2->start();

    thread3=new recvFileThread(sockfd3);
    thread3->start();

    connect(ui->tableWidget_userlist,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(getItem()));
    connect(ui->btn_one,SIGNAL(clicked()),this,SLOT(btn_one_slot()));
    connect(ui->btn_all,SIGNAL(clicked()),this,SLOT(btn_all_slot()));
    connect(ui->btn_send,SIGNAL(clicked()),this,SLOT(btn_send_slot()));
    connect(thread,SIGNAL(sendMsg_signal(Msg*)),this,SLOT(recv_msg(Msg*)));
    connect(thread2,SIGNAL(listupdate()),this,SLOT(update_Online_userinfo()));
    connect(ui->btn_file,SIGNAL(clicked()),this,SLOT(selectfile()));
}


void interface::selectfile()
{

    QFileDialog * fileDialog = new QFileDialog(this);
    //    fileDialog->setDirectory("D:\\");
    fileDialog->setWindowTitle(QString::fromLocal8Bit("选择文件"));
    fileDialog->setNameFilter(QString::fromLocal8Bit("All files (*)"));
    if ( fileDialog->exec() == QDialog::DialogCode::Accepted )
    {
        QString path = fileDialog->selectedFiles()[0];
        //        QMessageBox::information(nullptr, QString::fromLocal8Bit("选择的文件"), QString::fromLocal8Bit("文件路径: ") + path);
        filetransfer(path);
    }
    else
    {
        QMessageBox::information(nullptr, QString::fromLocal8Bit("选择的文件"), QString::fromLocal8Bit("没有选择文件"));
    }
    //    QString path = fileDialog->selectedFiles()[0];
    //
}

void interface::filetransfer(QString filename)
{

    //    msg.to_user_id = to_user_id;
    //    msg.from_user_name[0]=1;
    //    msg.from_user_name[1]=0;
    //    msg.from_user_name[2]=0;
    //    qDebug() << msg.from_user_name<< Qt::endl;
    //    send(sockfd,&msg,sizeof(msg),0);
    qDebug() <<"sending file : " << qPrintable(filename)<<Qt::endl;
    qDebug() <<to_user_id<<Qt::endl;
    file* transfer=new file(sockfd3,my_id,to_user_id,filename);
    qDebug()<<"Recv res!!! start sending!!!"<<Qt::endl;
    transfer->show();
    transfer->fileTransfer();



    //    fileName=filename;
}


interface::~interface()
{
    delete ui;
}

void interface::closeEvent(QCloseEvent *event)
{
    connectServer::closeSockfd(sockfd);
}

void interface::recv_Online_userinfo()
{
    int length = 0;

    recv(sockfd,&length,sizeof(int),0);
    qDebug() << "Current online user count is:" << length << Qt::endl;
    online_userinfo ouf;

    int i = 0;
    while(length--)
    {
        recv(sockfd,&ouf,sizeof(ouf),0);
        ui->tableWidget_userlist->insertRow(i);
        ui->tableWidget_userlist->setItem(i,0,new QTableWidgetItem(ouf.id));
        ui->tableWidget_userlist->setItem(i,1,new QTableWidgetItem(ouf.name));
        i++;
        qDebug() << "userID: " << ouf.id << "  name: " << ouf.name << Qt::endl;
    }

}

void interface::update_Online_userinfo()
{
    int length = 0;

    recv(sockfd2,&length,sizeof(int),0);
    qDebug() << "Current online user count is:" << length << Qt::endl;
    online_userinfo ouf;
    ui->tableWidget_userlist->clearContents();
    ui->tableWidget_userlist->setRowCount(0);
    int i = 0;
    while(length--)
    {
        recv(sockfd2,&ouf,sizeof(ouf),0);
        ui->tableWidget_userlist->insertRow(i);
        qDebug() <<"insert"<<Qt::endl;
        ui->tableWidget_userlist->setItem(i,0,new QTableWidgetItem(ouf.id));
        ui->tableWidget_userlist->setItem(i,1,new QTableWidgetItem(ouf.name));
        i++;
        qDebug() << "userID: " << ouf.id << "  name: " << ouf.name << Qt::endl;
    }

}

void interface::getItem()
{
    if(1 == chatType)
    {
        int row_number = ui->tableWidget_userlist->currentRow();
        to_user_id = ui->tableWidget_userlist->item(row_number,0)->data(Qt::DisplayRole).toString().toInt();
        to_user_name = ui->tableWidget_userlist->item(row_number,1)->data(Qt::DisplayRole).toString();
        ui->chatuser->setText(to_user_name);
        msg.to_user_id = to_user_id;
        ui->btn_send->setEnabled(true);
    }
}

void interface::btn_one_slot()
{
    ui->btn_all->setEnabled(true);
    ui->btn_one->setEnabled(false);
    this->ui->tableWidget_userlist->setSelectionMode(QAbstractItemView::SingleSelection);
    chatType = 1;

    ui->btn_send->setEnabled(false);

    delete_Msg();
}

void interface::btn_all_slot()
{
    ui->btn_all->setEnabled(false);
    ui->chatuser->setText("everyone");
    ui->btn_one->setEnabled(true);
    ui->btn_send->setEnabled(true);
    chatType = 0;
    to_user_id = 0;
    msg.to_user_id = to_user_id;

    delete_Msg();
}

void interface::btn_send_slot()
{
    QString msg_str = ui->lineEdit_msg->text();
    if(msg_str == "")
        return;
    msg.to_user_id = to_user_id;
    //    msg.from_user_name[0]=0;
    //    msg.from_user_name[1]=0;
    //    msg.from_user_name[2]=0;
    strcpy(msg.msg,msg_str.toStdString().c_str());
    send(sockfd,&msg,sizeof(msg),0);
    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(msg.msg) + " :我"));
    QTableWidgetItem* item = ui->tableWidget_msgPage->item(row_count,0);
    item->setTextAlignment(Qt::AlignRight);
}

void interface::recv_msg(Msg* msg)
{
    if(chatType == 1)//one mode
    {
        if(0 == msg->to_user_id)
        {
            return;
        }
        to_user_id = msg->from_user_id;
        ui->btn_send->setEnabled(true);
    }
    if(0 == chatType) //everyone mode
    {
        if(0 != msg->to_user_id)
        {
            return;
        }
    }

    if(msg->from_user_id == this->msg.from_user_id)
        return;
    //    qDebug()<<msg->from_user_name[0]<<Qt::endl;
    //    if(msg->from_user_name[0]==1&&msg->from_user_name[1]==0&&msg->from_user_name[2]==0)
    //    {
    //        qDebug()<<"hhhhhhhhh"<<Qt::endl;
    //        QMessageBox msgBox;
    //        msgBox.setText("接收文件提醒");
    //        msgBox.setInformativeText("确实要接收文件吗?");
    //        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    //        msgBox.setDefaultButton(QMessageBox::Ok);
    //        int ret = msgBox.exec();
    //        if(ret == QMessageBox::Ok){
    //            Msg res;
    //            res.from_user_id=my_id;
    //            res.to_user_id=msg->from_user_id;
    //            res.from_user_name[0]=2;
    //            res.from_user_name[1]=0;
    //            res.from_user_name[2]=0;
    //            qDebug()<<"hhhhhhhhh"<<Qt::endl;
    //            send(sockfd,&res,sizeof(res),0);
    //        }
    //    }
    //    else if(msg->from_user_name[0]==2&&msg->from_user_name[1]==0&&msg->from_user_name[2]==0)
    //    {
    //        file* transfer=new file(sockfd3,my_id,msg->from_user_id,fileName);
    //        qDebug()<<"Recv res!!! start sending!!!"<<Qt::endl;
    //        transfer->fileTransfer();
    //        transfer->show();
    //    }
    //    else{
    int row_count = ui->tableWidget_msgPage->rowCount();
    ui->tableWidget_msgPage->insertRow(row_count);
    ui->tableWidget_msgPage->setItem(row_count,0,new QTableWidgetItem(QString(msg->from_user_name) +": " + QString(msg->msg)));
    //    }

}

void interface::delete_Msg()
{
    int row_count = ui->tableWidget_msgPage->rowCount();
    while(row_count)
    {
        row_count--;
        ui->tableWidget_msgPage->removeRow(row_count);
    }
}
