#include "session.h"

Session::Session(boost::asio::ip::tcp::socket _socket)
    : stream{std::move(_socket)} {}

void Session::start() { do_read(); }

void Session::do_read() {
  http::async_read(stream, buffer, req,
                   std::bind(&Session::on_read, shared_from_this(),
                             std::placeholders::_1, std::placeholders::_2));
}

void Session::on_read(boost::system::error_code ec, std::size_t) {
  if (ec) {
    if (ec == http::error::end_of_stream) {
      std::cout << "EOS Error: " << ec.message() << std::endl;
      stream.close();
      return;
    }
    if (ec == http::error::bad_method) {
      std::cout << "BAD Method: " << ec.message() << std::endl;
      stream.close();
      return;
    }

    std::cout << ec.message() << std::endl;
  }

  if (beast::websocket::is_upgrade(req)) {
    std::make_shared<WsSession>(stream.release_socket())->start(req);
    return;
  }

  std::make_shared<HttpSession>(std::move(stream), std::move(req))->start();
}
