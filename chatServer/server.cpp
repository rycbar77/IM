#include "server.h"

Server::Server() {
    init();
    list = new UserList();
    db = new DBMysql("127.0.0.1", "root", "zarzarjason0330", "chatServer");
    db->db_connect();
}

Server::~Server() {
    delete db;
}


void Server::init() {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(9998);
    addr2.sin_addr.s_addr = htonl(INADDR_ANY);

    addr3.sin_family = AF_INET;
    addr3.sin_port = htons(9997);
    addr3.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        return;
    }

    int ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (-1 == ret) {
        close(sockfd);
        sockfd = -1;
        return;
    }
    ret = listen(sockfd, 1024);
    if (-1 == ret) {
        close(sockfd);
        sockfd = -1;
        return;
    }

    int ret2 = bind(sockfd2, (struct sockaddr *) &addr2, sizeof(addr2));
    if (-1 == ret2) {
        close(sockfd2);
        sockfd2 = -1;
        return;
    }
    ret2 = listen(sockfd2, 1024);
    if (-1 == ret2) {
        close(sockfd2);
        sockfd2 = -1;
        return;
    }


    int ret3 = bind(sockfd3, (struct sockaddr *) &addr3, sizeof(addr3));
    if (-1 == ret3) {
        close(sockfd3);
        sockfd3 = -1;
        return;
    }
    ret3 = listen(sockfd3, 1024);
    if (-1 == ret3) {
        close(sockfd3);
        sockfd3 = -1;
        return;
    }
}


bool Server::acceptClient() {
//    cout<<"test"<<endl;
    if (-1 == sockfd)
        return false;
//    cout<<"test00"<<endl;
    int csockfd = accept(sockfd, NULL, NULL);
    if (-1 == csockfd) {
        return false;
    }
//    cout<<"test11"<<endl;
    userInfo lf;

    ssize_t size = recv(csockfd, (void *) &lf, sizeof(lf), 0);
//    cout << "test" << endl;
    if (-1 == size || 0 == size) {
        close(csockfd);
        return false;
    }
    if (1 == lf.flag) {
        string sqlstr("select * from user where id = '");
        sqlstr.append(lf.id_name).append("'");
        char **row = db->db_select(sqlstr.c_str());
        int login_flag = 0;
        if (row == NULL) {
            login_flag = 0;
            size = send(csockfd, &login_flag, sizeof(int), 0);
            close(csockfd);
            return false;
        } else {
            if (!strcmp(lf.id_name, row[0]) && !strcmp(lf.pwd, row[2])) {
                cout << "confirmation successful" << endl;
                login_flag = 1;
                int csockfd2 = accept(sockfd2, NULL, NULL);
                int csockfd3 = accept(sockfd3, NULL, NULL);
                User user(lf.id_name, row[1], csockfd, csockfd2, csockfd3);
                list->push(user);
                list->show();

                size = send(csockfd, &login_flag, sizeof(int), 0);
                Thread *thread = new Thread(user, list);
                thread->start();
                cout << "user " << row[0] << ":" << row[1] << "  online...." << endl;
                return true;
            } else {
                login_flag = 0;
                size = send(csockfd, &login_flag, sizeof(int), 0);
                close(csockfd);
                return false;
            }
        }
    } else {
        cout << "user name:" << lf.id_name << endl << "user pwd:" << lf.pwd << endl;
        char account[10] = {0};
        ifstream in("auto_account.dat");
        in.read(account, sizeof(account));
        in.close();

        int ac = atoi(account);
        ac++;
        string str;
        stringstream ss;
        ss << ac;
        ss >> str;
        ofstream out("auto_account.dat");
        out.write(str.c_str(), strlen(str.c_str()));
        if (!out) {
            cout << "write fail" << endl;
        }
        cout << account << endl;

        send(csockfd, account, sizeof(account), 0);
        string sqlstr("insert into user values('");
        sqlstr.append(account).append("','").append(lf.id_name).append("','").append(lf.pwd).append("')");
        db->db_insert(sqlstr.c_str());
        close(csockfd);
    }
    return true;
}

void Server::closeServer() {
    close(sockfd);
    close(sockfd2);
    close(sockfd3);
}
