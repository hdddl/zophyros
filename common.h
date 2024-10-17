#ifndef __ZOPHYROS_COMMON_H__
#define __ZOPHYROS_COMMON_H__

#include <boost/beast/http/buffer_body.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/verb.hpp>

namespace zophyros
{

typedef boost::beast::http::request<boost::beast::http::buffer_body> HttpRequest;   
typedef boost::beast::http::response<boost::beast::http::string_body> HttpResponse;

const std::string APPLICATION_JSON = "application/json";


}

#endif