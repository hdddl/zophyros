#ifndef __ZOPHYROS_HTTP_SERVER_H__
#define __ZOPHYROS_HTTP_SERVER_H__

#include "muduo/net/TcpServer.h"

#include "httpRoute.h"
#include "common.h"

namespace zophyros
{

class HttpServer
{
public:
    HttpServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             const std::string& name,
             muduo::net::TcpServer::Option option = muduo::net::TcpServer::kNoReusePort);

    muduo::net::EventLoop* getLoop() const { return tcpServer_m.getLoop(); }

    void start();
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                    muduo::net::Buffer* buf,
                    muduo::Timestamp receiveTime);

    void onRequest(const muduo::net::TcpConnectionPtr& conn, const HttpRequest& message);
    
    std::unique_ptr<HttpRoute> httpRouter_m;

    // Lower layer TCP Server (thread number is set on this)
    muduo::net::TcpServer tcpServer_m;
};


}

#endif