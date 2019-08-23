//
// Created by silent on 19-8-22.
//

#include <mysql/mysql_pool.h>

MysqlPool* MysqlPool::mysqlPoolInstance = NULL;

MysqlPool* MysqlPool::GetInstance(const char* host, const char* user, const char* passwd, const char* db, int maxSize) {
    if (NULL == mysqlPoolInstance)
        mysqlPoolInstance = new MysqlPool(host, user, passwd, db, maxSize);

    return mysqlPoolInstance;
}

MysqlPool::MysqlPool(const char* host, const char* user, const char* passwd, const char* db, int maxSize) :
    port(0), unix_socket(NULL), clientflag(0)
{
    this -> host = host;
    this -> user = user;
    this -> passwd = passwd;
    this -> db = db;
    this -> maxSize = maxSize;
    this -> curSize = 0;
    this->InitPool(maxSize / 2);
}

MysqlPool* MysqlPool::GetInstance(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, int maxSize) {
    if (NULL == mysqlPoolInstance)
            mysqlPoolInstance = new MysqlPool(host, user, passwd, db, port, maxSize);

    return mysqlPoolInstance;
}

MysqlPool::MysqlPool(const char* host, const char* user, const char* passwd, const char* db, unsigned int port, int maxSize) :
    unix_socket(NULL), clientflag(0)
{
    this -> host = host;
    this -> user = user;
    this -> passwd = passwd;
    this -> db = db;
    this -> port = port;
    this -> maxSize = maxSize;
    this -> curSize = 0;
    this ->InitPool(maxSize / 2);
}

MysqlPool* MysqlPool::GetInstance(const char *host, const char *user, const char *passwd, const char *db,unsigned int port, const char *unixSocket, unsigned long clientFlag, int maxSize)
{
    if (NULL == mysqlPoolInstance)
        mysqlPoolInstance = new MysqlPool(host, user, passwd, db, port, unixSocket, clientFlag, maxSize);

    return mysqlPoolInstance;
}

MysqlPool::MysqlPool(const char *host, const char *user, const char *passwd, const char *db, unsigned int port,const char *unixSocket, unsigned long clientFlag, int maxSize)
{
    this -> host = host;
    this -> user = user;
    this -> passwd = passwd;
    this -> db = db;
    this -> port = port;
    this -> unix_socket = unixSocket;
    this -> clientflag = clientFlag;
    this -> maxSize = maxSize;
    this -> curSize = 0;
    this ->InitPool(maxSize / 2);
}

void MysqlPool::ErrorReport(const char* msg) {
    perror(msg);
}

void MysqlPool::InitPool(int iInitialSize) {
    MYSQL* conn;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < iInitialSize; i ++) {
        conn = this->CreateConnection();
        if(conn) {
            connList.push_back(conn);
            ++(this -> curSize);
        } else {
            this->ErrorReport("MysqlPool::InitPool, fail to create connection.");
        }
    }
    pthread_mutex_unlock(&lock);
}

MYSQL* MysqlPool::CreateConnection() {
    MYSQL* conn;
    try {
        conn = mysql_init((MYSQL*) 0);
        mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");
        mysql_real_connect(conn, this->host, this->user, this->passwd, this->db, this->port, this->unix_socket, this->clientflag);
        return conn;
    } catch (std::runtime_error &e) {
        this -> ErrorReport("MysqlPool::CreateConnection, runtime error.");
        return NULL;
    } catch (const exception &e) {
        this -> ErrorReport("MysqlPool::CreateConnection, fail to execute mysql_real_connect to create a mysql connect.");
        return NULL;
    }

}

MysqlPool::~MysqlPool() {
    this -> DestoryPool();
}

MYSQL* MysqlPool::GetConnection() {
    MYSQL* conn;
    pthread_mutex_lock(&lock);

    if (connList.size() > 0) {
        conn = connList.front();
        connList.pop_front();
        if (NULL == conn) {
            delete conn;
            conn = this->CreateConnection();
        }

        if (NULL == conn) {
            --curSize;
        }
        pthread_mutex_unlock(&lock);
        return conn;

    } else {

        if ( curSize < maxSize && (conn = this -> CreateConnection())) {
            ++curSize;
            pthread_mutex_unlock(&lock);
            return conn;
        } else {
            pthread_mutex_unlock(&lock);
            return NULL;
        }

    }
}

void MysqlPool::ReleassePool(MYSQL *conn) {
    if (conn) {
        pthread_mutex_lock(&lock);
        connList.push_back(conn);
        pthread_mutex_unlock(&lock);
    }
}

void MysqlPool::DestoryPool() {
    list<MYSQL*>::iterator it;
    pthread_mutex_lock(&lock);
    for (it = connList.begin(); it != connList.end(); ++it) {
        this -> DestoryConnection(*it);
    }
    curSize = 0;
    connList.clear();
    pthread_mutex_unlock(&lock);

}

void MysqlPool::DestoryConnection(MYSQL *conn) {
    if (conn) {
        try {
            mysql_close(conn);
        } catch (std::exception &e) {
            this -> ErrorReport(e.what());
        } catch (const exception &e) {
            this -> ErrorReport(e.what());
        }
        delete  conn;
    }
}