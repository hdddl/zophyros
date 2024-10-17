#include "../httpServer.h"
#include <boost/asio/io_context.hpp>
#include <nlohmann/json.hpp>
#include "muduo/net/EventLoop.h"
#include <boost/beast/http/field.hpp>

#include "muduo/base/Logging.h"

using namespace boost::beast;
using namespace muduo::net;
using namespace zophyros;

using json = nlohmann::json;

void handleGET(const HttpRequest& httpRequest, HttpResponse& response)
{
    json tt = {
        {"Hello", "world"}
    };

    response.body() = tt.dump();
    response.set(http::field::content_type, APPLICATION_JSON);
}

void handlePost(const HttpRequest& httpRequest, HttpResponse& response)
{
    LOG_INFO << "body size: " << httpRequest.body().size;

    

    const auto& body_data = httpRequest.body().data;
    std::string requestBody(reinterpret_cast<const char*>(body_data), httpRequest.body().size);

    LOG_INFO << requestBody;

    httpRequest.body().data;

}

void test() 
{
    
}


int main()
{
    EventLoop loop;

    HttpRoutePtr route = std::make_unique<HttpRoute>();
    route->GET("/", handleGET);
    route->POST("/", handlePost);

    HttpServer server(
                    &loop, 
                    InetAddress("127.0.0.1", 2323), 
                    "demoHttpServer", 
                    route);
    server.start();


    loop.loop();


    return 0;
}