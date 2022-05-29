#include "listener.h"

#include "session.h"

Listener::Listener(boost::asio::io_context& _ioc)
    : acceptor{std::make_shared<net::ip::tcp::acceptor>(
          _ioc,
          net::ip::tcp::endpoint(net::ip::tcp::v4(), 5000))} {
  do_accept();
}

void Listener::do_accept() {
  std::cout << __FUNCTION__ << std::endl;
  acceptor->async_accept(
      [this](boost::system::error_code ec, net::ip::tcp::socket socket) {
        if (ec) {
          std::cout << "[e] Error in listening connection.";
          do_accept();
        }
        std::make_shared<Session>(std::move(socket))->start();
        do_accept();
      });
}
