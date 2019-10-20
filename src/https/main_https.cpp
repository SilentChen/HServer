#include "server_https.hpp"
#include "handler.hpp"
using namespace HServer;

int main() {
    //HTTPS 服务运行在 12345 端口，并启用四个线程
    Server<HTTPS> server(233, 4, "server.crt", "server.key");
    start_server<Server<HTTPS>>(server);
    return 0;
}