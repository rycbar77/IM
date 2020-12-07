#ifndef SERVER_H
#define SERVER_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "user.h"
#include "userlist.h"
#include "dbmysql.h"
#include "thread.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Server
{
public:
    Server();
    ~Server();


    bool acceptClient();
    void closeServer();
private:
    struct sockaddr_in addr;
    struct sockaddr_in addr2;
    struct sockaddr_in addr3;
    int sockfd;
    int sockfd2;
    int sockfd3;
    UserList* list;
    DBMysql* db;
    void init();
};

#endif // SERVER_H
