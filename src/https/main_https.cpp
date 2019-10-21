// main_https.cpp
// web_server
// created by silent at silent.com
//

#include "server_https.hpp"
#include "handler.hpp"
#include "json/json.h"
#include "mysql/mysql_pool.h"
#include "reflect_func.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "home.hpp"

using namespace HServer;

void init();
void registerFunc();


struct cfg_mysql {
	char* host;
	char* user;
	char* passwd;
	char* dbname;
	int pool_num;
};

typedef cfg_mysql cfg_mysql;

struct cfg_https {
	int port;
	int thread_num;
};

typedef cfg_https cfg_https;

struct cfg {
	cfg_mysql mysql;
	cfg_https https;
}g_cfg;

MysqlPool* mypoolInstance;

void writeJson();

void loadConfig() {
	ifstream ifs;
	ifs.open("config/app.config");
	if(!ifs.is_open()) 
	{
		cout << "fail to open file" << endl;
		return;
	}
	Json::Reader jsonReader;
	Json::Value value;
	if (jsonReader.parse(ifs, value)) 
	{
		// cout << "json load is: " << value.toStyledString() << endl;
		g_cfg.mysql.host	= new char[value["mysql"]["host"].asString().length()];
		strcpy(g_cfg.mysql.host, value["mysql"]["host"].asString().c_str());

		g_cfg.mysql.user	 = new char[value["mysql"]["user"].asString().length()];
		strcpy(g_cfg.mysql.user, value["mysql"]["usr"].asString().c_str());

		g_cfg.mysql.passwd	= new char[value["mysql"]["passwd"].asString().length()];
		strcpy(g_cfg.mysql.passwd, value["mysql"]["passwd"].asString().c_str());

		g_cfg.mysql.dbname	= new char[value["mysql"]["dbname"].asString().length()];
		strcpy(g_cfg.mysql.dbname, value["mysql"]["dbname"].asString().c_str());

		g_cfg.mysql.pool_num = value["mysql"]["pool_num"].asInt();

		g_cfg.https.port = value["https"]["port"].asInt();
		g_cfg.https.thread_num = value["https"]["thread_num"].asInt();
	}
}



int main() {
	init();
	mypoolInstance = MysqlPool::GetInstance(g_cfg.mysql.host, g_cfg.mysql.user, g_cfg.mysql.passwd, g_cfg.mysql.dbname, g_cfg.mysql.pool_num);
    writeJson();
	Server<HTTPS> server(g_cfg.https.port, g_cfg.https.thread_num, "cert/server.crt", "cert/server.key");
	start_server<Server<HTTPS>>(server);

	return 0;
}

void init() {
	registerFunc();
	loadConfig();
}

void registerFunc() {
	FUNC_REGISTER(FlutterHome_index);
}

void writeJson() {
    Json::Value data1;
    Json::Value data2;
    Json::Value row;
    Json::Value arrayObj;
    MYSQL_ROW myrow;
    MYSQL* conn = mypoolInstance -> GetConnection();
    const char* sql = "select pkey,nickname,lv,vip_lv from player_state";
    int state = mysql_real_query(conn, sql, strlen(sql));
	if (state > 0) 
	{
		cout << "error occur: " << mysql_error(conn) << endl;
		return;
	}
	cout << "query return state is: " << state << endl;
    MYSQL_RES* res = mysql_store_result(conn);
	cout << "row count is: " << res -> row_count << endl;
    int i,j = 0;
    while (myrow = mysql_fetch_row(res)) {
        for (i=0; i < res->field_count; i++)
        {
            row[i] = myrow[i];
        }
        arrayObj.append(row);
        data1[j] = row;
        data2[j] = arrayObj;
        j++;
    }
    data1.toStyledString();
    std::string out1 = data1.toStyledString();
    std::cout << out1 << std::endl;

    data2.toStyledString();
    std::string out2 = data2.toStyledString();
    std::cout << out2 << std::endl;
}