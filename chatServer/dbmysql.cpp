#include "dbmysql.h"

DBMysql::DBMysql(const char *host, const char *user, const char *passwd, const char *db_name)
        : host(host), user(user), passwd(passwd), db_name(db_name) {

}

DBMysql::~DBMysql() = default;

int DBMysql::db_connect() {
    if (nullptr == mysql_init(&mysql)) {
        cout << "mysql_init(): " << mysql_error(&mysql) << endl;
        return -1;
    }

    if (nullptr == mysql_real_connect(&mysql, host, user, passwd, db_name, 0, nullptr, 0)) {
        cout << "mysql_real_connect():" << mysql_error(&mysql) << endl;
        return -1;
    }

    cout << "Connected MySQL successful! \n" << endl;
    return 0;
}

void DBMysql::db_insert(const char *sqlstr) {
    int flag = mysql_real_query(&mysql, sqlstr, strlen(sqlstr));
    if (flag != 0)
    {
        cout << " mysql_real_query(): " << mysql_error(&mysql) << endl;
        return;
    }
}

MYSQL_ROW DBMysql::db_select(const char *sqlstr) {
    int flag = mysql_real_query(&mysql, sqlstr, strlen(sqlstr));
    if (flag != 0)
    {
        cout << " mysql_real_query(): " << mysql_error(&mysql) << endl;
        return nullptr;
    }

    MYSQL_RES *res = mysql_store_result(&mysql);
    if (nullptr == res) {
        printf("mysql_restore_result(): %s\n", mysql_error(&mysql));
        return nullptr;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    return row;
}

void DBMysql::db_close() {
    mysql_close(&mysql);
}
