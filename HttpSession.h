#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/json/serialize.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
  boost::beast::tcp_stream stream;
  http::request<http::string_body> req;
  boost::system::error_code ec;

 public:
  HttpSession(boost::beast::tcp_stream _stream,
              http::request<http::string_body> _req);
  void start();
  void handle_request();
  void get_request();
  void post_request();
  json::object parse_json();
  void send_response();
};
#endif  // HTTPSESSION_H
