#ifndef WSSESSION_H
#define WSSESSION_H

#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <boost/json/parse.hpp>
#include <iostream>
#include <optional>

#include "Handler/HandlerManager.h"
#include "TException.h"

class User;

namespace net = boost::asio;
namespace beast = boost::beast;
namespace ws = beast::websocket;
namespace json = boost::json;

class WsSession : public std::enable_shared_from_this<WsSession> {
  ws::stream<beast::tcp_stream> ws;
  beast::flat_buffer buffer;
  std::shared_ptr<User> _currentUser;
  HandlerManager* handler;

 public:
  WsSession(net::ip::tcp::socket socket);
  ~WsSession();
  void start(beast::http::request<beast::http::string_body> req);
  void do_read();
  void on_read(boost::system::error_code ec, std::size_t size);
  std::optional<json::object> parse_message(size_t& size);
  void on_accept(boost::system::error_code ec);
  void handle_request(json::object& request, json::object& response);
  void handle_event(json::object& request, json::object& response_data);
  void do_write(const std::string& data);
  void do_write(json::object jobj);
  void on_write(boost::system::error_code ec, std::size_t size);
  const std::string jsonToString(json::object jobj);
  const std::shared_ptr<User>& currentUser() const;

  json::object createBoard(json::object& request);
  json::object getBoardStats(json::object& request);
  json::object pickNumber(json::object& request);
  json::object getBoardById(json::object& request);
  json::object getBoardByUid(json::object& request);
  json::object getBoard(json::object& request);

  json::object signUp(json::object& request);
  json::object signIn(json::object& request);
  json::object manualSignIn(json::object& request);
  json::object logout(json::object& request);
  json::object getAllUsers(json::object& request);

  json::object createTicket(json::object& request);
  json::object createTicketByBid(json::object& request);
  json::object createTicketByGS(json::object& request);
  json::object createTicketSet(json::object& request);
  json::object getTicket(json::object& request);
  json::object getTicketById(json::object& request);
  json::object getTicketByUid(json::object& request);

  json::object createGameSession(json::object& request);
  json::object getGameSessionById(json::object& request);
  json::object getGameSessionByUid(json::object& request);
  json::object getGameSessionByTid(json::object& request);

  inline json::object createResponse(json::object& request,
                                     json::object data,
                                     int16_t error_code = 0,
                                     const std::string message = "");
  void setCurrentUser(const std::shared_ptr<User>& newCurrentUser);
};

#endif  // WSSESSION_H
