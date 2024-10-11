#include "../httpServer.h"

#include "muduo/net/EventLoop.h"

using namespace muduo::net;
using namespace zophyros;

int main()
{
    EventLoop loop;
    HttpServer server(&loop, InetAddress("127.0.0.1", 2323), "demoHttpServer");
    server.start();


    loop.loop();


    return 0;
}