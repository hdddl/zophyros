#include "httpRoute.h"
#include "common.h"
#include "muduo/base/Logging.h"

using namespace zophyros;

HttpRoute::HttpRoute()
        : callbacks_m()
{

}

HttpRoute::~HttpRoute()
{

}

void HttpRoute::GET(const std::string &path, const HttpCallbackFunc &callbackFunc)
{
    callbacks_m[boost::beast::http::verb::get][path] = callbackFunc;
}

void HttpRoute::POST(const std::string &path, const HttpCallbackFunc &callbackFunc)
{
    callbacks_m[boost::beast::http::verb::post][path] = callbackFunc;
}

void HttpRoute::DELETE(const std::string &path, const HttpCallbackFunc &callbackFunc)
{
    callbacks_m[boost::beast::http::verb::delete_][path] = callbackFunc;
}

void HttpRoute::PUT(const std::string &path, const HttpCallbackFunc &callbackFunc)
{
    callbacks_m[boost::beast::http::verb::put][path] = callbackFunc;    
}

HttpRoute::HttpCallbackFunc HttpRoute::getCallback(const boost::beast::http::verb& method, const boost::beast::string_view& path)
{
    auto it = callbacks_m.find(method);
    if (it == callbacks_m.end())
    {
        LOG_ERROR << "Callback function not found for method: " << boost::beast::http::to_string(method);
        return HttpRoute::defaultNotFoundHandler;
    }

    auto it2 = it->second.find(path);
    if (it2 == it->second.end())
    {
        LOG_ERROR << "Callback function not found for path: " << path;
        return HttpRoute::defaultNotFoundHandler;
    }

    return it2->second;   
}

void HttpRoute::defaultNotFoundHandler(const HttpRequest& request, HttpResponse& response)
{
    LOG_DEBUG << "Method Entry";

    response.set(boost::beast::http::field::content_type, "application/json");
    response.body() = "{\"error\": \"Not Found\"}";

    response.result(boost::beast::http::status::not_found);
}

void HttpRoute::defaultServerErrorHandler(const HttpRequest& request, HttpResponse& response)
{
    LOG_DEBUG << "Method Entry";

    response.set(boost::beast::http::field::content_type, "application/json");
    response.body() = "{\"error\": \"Internal Server Error\"}";

    response.result(boost::beast::http::status::internal_server_error);
}