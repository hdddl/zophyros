#ifndef __ZOPHYROS_HTTP_SERVER_H__
#define __ZOPHYROS_HTTP_SERVER_H__

#include "muduo/net/TcpServer.h"

#include "httpRoute.h"
#include "common.h"

namespace zophyros
{

class HttpRoute;
typedef std::unique_ptr<HttpRoute> HttpRoutePtr;
class HttpServer
{
public:
    HttpServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             const std::string& name,
             HttpRoutePtr& route,
             muduo::net::TcpServer::Option option = muduo::net::TcpServer::kNoReusePort);

    void start();
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp receiveTime);

    void onRequest(const muduo::net::TcpConnectionPtr& conn, const HttpRequest& message);
    
    HttpRoutePtr httpRouter_m;

    // Lower layer TCP Server (thread number is set on this)
    muduo::net::TcpServer tcpServer_m;
};


}

#endif