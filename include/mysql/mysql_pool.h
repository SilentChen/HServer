//
// Created by silent on 19-8-21.
//

#ifndef HTTP_MYSQL_H
#define HTTP_MYSQL_H

#include <mysql.h>
#include <list>
#include <pthread.h>
#include <iostream>

using namespace std;

class MysqlPool {
private:
    // param and attribute
    int curSize;
    int maxSize;
    const char* host;
    const char* user;
    const char* passwd;
    const char* db;
    unsigned int port;
    const char* unix_socket;
    unsigned long clientflag;
    list<MYSQL*> connList;
    pthread_mutex_t lock;
    static MysqlPool* mysqlPoolInstance;

    // function
    MYSQL* CreateConnection();
    void InitPool(int iInitialSize);
    void DestoryPool();
    void DestoryConnection(MYSQL* conn);
    MysqlPool(const char* host, const char* user, const char* passwd, const char* db, int maxSize);
    MysqlPool(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, int maxSize);
    MysqlPool(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, const char* unixSocket, unsigned long clientflag, int maxSize);
    void ErrorReport(const char* msg);

public:
    virtual ~MysqlPool();
    MYSQL* GetConnection();
    void ReleassePool(MYSQL* conn);
    static MysqlPool* GetInstance(const char* host, const char* user, const char* passwd, const char* db, int maxSize);
    static MysqlPool* GetInstance(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, int maxSize);
    static MysqlPool* GetInstance(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, const char* unixSocket, unsigned long clientFlag, int maxSize);
};

#endif //HTTP_MYSQL_H
