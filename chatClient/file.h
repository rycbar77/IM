#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QString>
#include "connectserver.h"
#include "msg.h"
namespace Ui {
class file;
}

class file : public QWidget
{
    Q_OBJECT

public:
    explicit file(int sockfd,int my_id,int to_id,QString filename,QWidget *parent = nullptr);
    ~file();
    void fileTransfer();
    unsigned long get_file_size(const char *path)
    {
        unsigned long filesize = -1;
        FILE *fp;
        fp = fopen(path, "r");
        if(fp == NULL)
            return filesize;
        fseek(fp, 0L, SEEK_END);
        filesize = ftell(fp);
        fclose(fp);
        return filesize;
    };


private:
    Ui::file *ui;
    QString filename;
    int sockfd;
    int my_id;
    int to_id;
    Msg msg;
};

#endif // FILE_H
