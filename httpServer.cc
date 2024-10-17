#include "httpServer.h"
#include "writebufLambda.hpp"
#include "httpRoute.h"

#include "muduo/base/Logging.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/buffer_body.hpp>
#include <boost/beast/http/error.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/none.hpp>
#include <cstddef>


using namespace zophyros;
using namespace muduo::net;
using namespace std;
using namespace std::placeholders;

HttpServer::HttpServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       const string& name,
                       HttpRoutePtr& httpRouter,
                       TcpServer::Option option)
  : httpRouter_m(std::move(httpRouter)),
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

    LOG_INFO << "readableBytes: " << buf->readableBytes();

    boost::beast::http::request_parser<
                        boost::beast::http::buffer_body> parser;
    char buf_t[512];
    parser.get().body().data = buf_t;
    parser.get().body().size = sizeof(buf_t);

    // ToDo: handle error
    parser.eager(true);
    boost::beast::error_code ec;
    std::size_t n = parser.put(
        boost::beast::net::buffer(buf->peek(), buf->readableBytes()), ec);

    // Set up the body for reading.
    // This is how much was parsed:
    parser.get().body().data = buf_t;
    parser.get().body().size = sizeof(buf_t) - parser.get().body().size;
    
    onRequest(conn, parser.get());
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& request)
{
    auto method = request.method();
    auto path = request.target();

    HttpResponse resp;

    // find callback function
    auto callback = httpRouter_m->getCallback(method, path);
    callback(request, resp);

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