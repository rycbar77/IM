#ifndef USERLIST_H
#define USERLIST_H
#include <string>
#include <iostream>
#include "user.h"
#include <cstring>

using namespace std;

struct Node
{
    User user;
    Node* next;
};

class UserList
{
public:
    UserList();
    ~UserList();

    void push(User user);
    void pop(int csockfd);
    int length();
    int selectSockfd(const char *id);
    int selectSockfd2(const char *id);
    int selectSockfd3(const char *id);
    void show();
    Node* & get_head_node();

private:
    Node* head;
    int m_length;


};

#endif // USERLIST_H
