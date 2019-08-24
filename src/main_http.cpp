
// main_http.cpp
// web_server
// created by changkun at shiyanlou.com
//

#include "server_http.hpp"
#include "handler.hpp"
#include "json/json.h"
#include "mysql/mysql_pool.h"

using namespace HServer;

MysqlPool* mypoolInstance = MysqlPool::GetInstance("127.0.0.1","root","root","test",10);

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

   /* Json::Value root;
    Json::Value arrayObj;
    Json::Value item;

    item["cpp"] = "jsoncpp";
    item["java"] = "jsoninjava";
    item["php"] = "support";
    arrayObj.append(item);

    root["name"] = "json";
    root["array"] = arrayObj;

    root.toStyledString();
    std::string out = root.toStyledString();
    std::cout << out << std::endl;*/
}


int main() {
    writeJson();
	// HTTP 服务运行在 12345 端口，并启用四个线程
	Server<HTTP> server(12345, 4);
	start_server<Server<HTTP>>(server);

	return 0;
}
