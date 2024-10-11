#ifndef __ZOPHYROS_HTTP_ROUTE_H__
#define __ZOPHYROS_HTTP_ROUTE_H__

#include "common.h"
#include <map>

namespace zophyros
{

class HttpRoute
{
    typedef std::function<void(const HttpRequest&, HttpResponse&)> HttpCallbackFunc;

    typedef std::map<boost::beast::http::verb, 
                    std::map<boost::beast::string_view, HttpCallbackFunc>> CallbackFuncsMap_m;
public:
    HttpRoute();
    ~HttpRoute();

    // using for register callback functions
    void GET(const std::string& path, const HttpCallbackFunc& callbackFunc);
    void POST(const std::string& path, const HttpCallbackFunc& callbackFunc);
    void PUT(const std::string& path, const HttpCallbackFunc& callbackFunc);
    void DELETE(const std::string& path, const HttpCallbackFunc& callbackFunc);

    HttpCallbackFunc getCallback(const boost::beast::http::verb& method, const boost::beast::string_view& path);
private:
    CallbackFuncsMap_m callbacks_m;

    // if no callback function found, return this function
   static void defaultNotFoundHandler(const HttpRequest& request, HttpResponse& response);
};

}

#endif