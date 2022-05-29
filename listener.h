#ifndef LISTENER_H
#define LISTENER_H

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>
#include <memory>

namespace net = boost::asio;

class Listener {
  std::shared_ptr<net::ip::tcp::acceptor> acceptor;

 public:
  Listener(net::io_context &_ioc);
  void do_accept();
};

#endif  // LISTENER_H
