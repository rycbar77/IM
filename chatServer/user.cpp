#include "user.h"

User::User(string id, string name, int csockfd,int csockfd2,int csockfd3):
    id(id),name(name),csockfd(csockfd),csockfd2(csockfd2),csockfd3(csockfd3)
{

}

User::~User()
{

}
