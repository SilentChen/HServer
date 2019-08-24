
// main_http.cpp
// web_server
// created by changkun at shiyanlou.com
//

#include "server_http.hpp"
#include "handler.hpp"
#include "json/json.h"
#include "mysql/mysql_pool.h"

#include <fstream>
#include <iostream>

using namespace HServer;


struct cfg_mysql {
	char* host;
	char* user;
	char* passwd;
	char* dbname;
}g_cfg_mysql;

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
		g_cfg_mysql.host	= new char[value["mysql"]["host"].asString().length()];
		strcpy(g_cfg_mysql.host, value["mysql"]["host"].asString().c_str());

		g_cfg_mysql.user	 = new char[value["mysql"]["user"].asString().length()];
		strcpy(g_cfg_mysql.user, value["mysql"]["usr"].asString().c_str());

		g_cfg_mysql.passwd	= new char[value["mysql"]["passwd"].asString().length()];
		strcpy(g_cfg_mysql.passwd, value["mysql"]["passwd"].asString().c_str());

		g_cfg_mysql.dbname	= new char[value["mysql"]["dbname"].asString().length()];
		strcpy(g_cfg_mysql.dbname, value["mysql"]["dbname"].asString().c_str());
	}
}



int main() {
	loadConfig();
	mypoolInstance = MysqlPool::GetInstance(g_cfg_mysql.host, g_cfg_mysql.user, g_cfg_mysql.passwd, g_cfg_mysql.dbname, 10);
    writeJson();
	// HTTP 服务运行在 12345 端口，并启用四个线程
	Server<HTTP> server(12345, 4);
	start_server<Server<HTTP>>(server);

	return 0;
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
