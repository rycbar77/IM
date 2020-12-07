#include "thread.h"

Thread::Thread(User user, UserList *list) :
        m_user(user), m_list(list) {

}

Thread::~Thread() {

}

void Thread::start() {
    cout << "starting thread..." << endl;

    int ret = pthread_create(&thread, NULL, run, this);
    if (0 != ret)
        cout << "create thread is fail" << endl;

    int ret2 = pthread_create(&thread2, NULL, runFile, this);
    if (0 != ret2)
        cout << "create thread is fail" << endl;
}

void *Thread::run(void *arg) {
    cout << "thread is started" << endl;
    Thread *th = (Thread *) (arg);
    int length = th->m_list->length();
    cout << "length" << length << endl;
    send(th->m_user.csockfd, &length, sizeof(int), 0);

    online_userinfo uf;
    Node *cur_node = th->m_list->get_head_node()->next;
    while (cur_node != NULL) {
        memset(&uf, 0, sizeof(uf));
        strcpy(uf.id, cur_node->user.id.c_str());
        strcpy(uf.name, cur_node->user.name.c_str());
        send(th->m_user.csockfd, &uf, sizeof(uf), 0);
        cur_node = cur_node->next;

    }
    cout << "send finish" << endl;

    Msg msg;
    while (1) {
        int ret = recv(th->m_user.csockfd, &msg, sizeof(msg), 0);
        if (-1 == ret || 0 == ret) {
            th->m_list->pop(th->m_user.csockfd);
            cout << "user " << th->m_user.id << ":" << th->m_user.name << " offline" << endl;
            close(th->m_user.csockfd);
            close(th->m_user.csockfd2);
            close(th->m_user.csockfd3);
            break;
        }


//        if (msg.from_user_name[0] != 1 && msg.from_user_name[0] != 2) {
        strcpy(msg.from_user_name, th->m_user.name.c_str());
//        }

        if (0 == msg.to_user_id) {
            cout << "recv a message from " << msg.from_user_id << ": " << msg.msg << endl;
            Node *cur_node = th->m_list->get_head_node()->next;
            while (cur_node != NULL) {
                send(cur_node->user.csockfd, &msg, sizeof(msg), 0);
                cout << "send to " << cur_node->user.id << " finish" << endl;
                cur_node = cur_node->next;
            }
        } else if (msg.to_user_id == -1) {
            online_userinfo uf;
            int length = th->m_list->length();
            send(th->m_user.csockfd2, &length, sizeof(int), 0);
            Node *cur_node = th->m_list->get_head_node()->next;
            while (cur_node != NULL) {
                memset(&uf, 0, sizeof(uf));
                strcpy(uf.id, cur_node->user.id.c_str());
                strcpy(uf.name, cur_node->user.name.c_str());
                send(th->m_user.csockfd2, &uf, sizeof(uf), 0);
                cur_node = cur_node->next;
            }
            cout << "send finish" << endl;
        } else {
            string str;
            stringstream ss;
            ss << msg.to_user_id;
            ss >> str;
            cout << msg.to_user_id << endl;
            int csockfd = th->m_list->selectSockfd(str.c_str());
            int ret = send(csockfd, &msg, sizeof(msg), 0);
            cout << csockfd << endl;
            if (-1 == ret || 0 == ret) {
                cout << "send msg to someone fail" << endl;
            }
        }
    }
    cout << "thread is die" << endl;
    return NULL;
}

void *Thread::runFile(void *arg) {
    cout << "thread is started" << endl;
    Thread *th = (Thread *) (arg);
    Msg msg;
    while (1) {
        int ret = recv(th->m_user.csockfd3, &msg, sizeof(msg), 0);
        if (-1 == ret || 0 == ret) {
            th->m_list->pop(th->m_user.csockfd);
            cout << "user " << th->m_user.id << ":" << th->m_user.name << " offline" << endl;
            close(th->m_user.csockfd);
            break;
        }


//        if (msg.from_user_name[0] != 1 && msg.from_user_name[0] != 2) {
        strcpy(msg.from_user_name, th->m_user.name.c_str());
//        }


        string str;
        stringstream ss;
        ss << msg.to_user_id;
        ss >> str;
        cout << msg.to_user_id << endl;
        int csockfd = th->m_list->selectSockfd3(str.c_str());
        ret = send(csockfd, &msg, sizeof(msg), 0);
        cout << csockfd << endl;
        if (-1 == ret || 0 == ret) {
            cout << "send file to someone fail" << endl;
        }
        int filesize = 0;
        ret = recv(th->m_user.csockfd3, &filesize, sizeof(filesize), 0);
        ret = send(csockfd, &filesize, sizeof(filesize), 0);

        char buffer[4096];
        while (1) {
            ret = recv(th->m_user.csockfd3, buffer, sizeof(buffer), 0);
            cout << ret << endl;
            if (ret == 0)
                break;
            ret = send(csockfd, buffer, ret, 0);
        }

    }
    cout << "thread is die" << endl;
    return NULL;
}
