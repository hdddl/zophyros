#include "httpServer.h"
#include "writebufLambda.hpp"
#include "httpRoute.h"

#include "muduo/base/Logging.h"

#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/serializer.hpp>


using namespace zophyros;
using namespace muduo::net;
using namespace std;
using namespace std::placeholders;

HttpServer::HttpServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const string& name,
                       TcpServer::Option option)
  : httpRouter_m(new HttpRoute()),
    tcpServer_m(loop, listenAddr, name, option)
{
    tcpServer_m.setConnectionCallback(
      std::bind(&HttpServer::onConnection, this, _1));
    tcpServer_m.setMessageCallback(
      std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
    LOG_WARN << "HttpServer[" << tcpServer_m.name()
        << "] starts listening on " << tcpServer_m.ipPort();
    tcpServer_m.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    LOG_TRACE << "Method Entry";
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           muduo::Timestamp receiveTime)
{
    LOG_TRACE << "Method Entry";

    // boost mutable buffer, not copy underlying data
    boost::asio::mutable_buffer tBuf(
                                    (void *)buf->peek(), buf->readableBytes());
    
    // parser http request
    boost::beast::http::request_parser<boost::beast::http::buffer_body> parser;
    boost::beast::error_code ec;
    size_t n = parser.put(tBuf, ec);
    if(n == 0)
    {
        LOG_ERROR  << "parse failed, err" << ec.message();
        return;
    }

    // parser http request success, begin process it
    LOG_INFO << "parse success";
    onRequest(conn, parser.get());
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& message)
{
    auto method = message.method();
    auto path = message.target();


    HttpResponse resp;

    // find callback function
    auto callback = httpRouter_m->getCallback(method, path);
    callback(message, resp);

    // encoding http response
    resp.prepare_payload();

    // http reponse serializer
    boost::beast::http::serializer<false, boost::beast::http::string_body> sr(resp);

    boost::beast::error_code ec;
    Buffer buf;
    WriteBufLambda<decltype(sr)> la(buf, sr);

    do
    {
        sr.next(ec, la);
        if(ec)
        {
            LOG_ERROR << "Failed to write to buffer";
            return;
        }
    }
    while(!sr.is_done());

    LOG_INFO << "Ok, we will send response, n = " << buf.readableBytes();

    conn->send(buf.peek(), buf.readableBytes());
}

