#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <memory>

#include "HttpSession.h"
#include "WsSession.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;

class Session : public std::enable_shared_from_this<Session> {
  beast::tcp_stream stream;
  beast::flat_buffer buffer;
  beast::http::request<http::string_body> req;

 public:
  explicit Session(net::ip::tcp::socket _socket);
  void start();
  void do_read();
  void on_read(boost::system::error_code ec, std::size_t size);
};

#endif  // SESSION_H
