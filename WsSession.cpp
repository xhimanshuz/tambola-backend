#include "WsSession.h"
#include "User/User.h"

void WsSession::setCurrentUser(const std::shared_ptr<User>& newCurrentUser) {
  _currentUser = newCurrentUser;
}

WsSession::WsSession(net::ip::tcp::socket socket) : ws{std::move(socket)} {
  handler = HandlerManager::get();
}

WsSession::~WsSession() {}

void WsSession::start(beast::http::request<beast::http::string_body> req) {
  ws.set_option(ws::stream_base::timeout::suggested(beast::role_type::server));
  //  ws.set_option(ws::stream_base::decorator([](ws::response_type &res) {
  //    res.set(beast::http::field::server,
  //            std::string(BOOST_BEAST_VERSION_STRING) + "websocket-test---");
  //  }));

  ws.accept(req);
  //  ws.async_accept(buffer, std::bind(&WsSession::on_accept,
  //  shared_from_this(),
  //                                    std::placeholders::_1));
  do_read();
}

void WsSession::handle_request(json::object& request, json::object& response) {
  const std::string type = request["type"].as_string().c_str();
  if (type == "event") {
    handle_event(request, response);
  }
}

void WsSession::handle_event(json::object& request, json::object& response) {
  if (!request.contains("event")) {
    request["event"] = "null";
    throw TException(400, "event key not found.");
  }
  const std::string event = request["event"].as_string().c_str();

  // Board
  if (event == "createBoard") {
    response = createBoard(request);
  } else if (event == "getBoardStats") {
    response = getBoardStats(request);
  } else if (event == "pickNumber") {
    response = pickNumber(request);

    // User Auth
  } else if (event == "signUp") {
    response = signUp(request);
  } else if (event == "signIn") {
    response = signIn(request);
  } else if (event == "logout") {
    response = logout(request);
  } else if (event == "getAllUsers") {
    response = getAllUsers(request);
  }

  // Ticket
  else if (event == "createTicket") {
    response = createTicket(request);
  } else if (event == "getTicketById") {
    response = getTicketById(request);  // for testing
  } else if (event == "getTicketByUid") {
    response = getTicketByUid(request);  // for testing
  } else if (event == "createTicketSet") {
    response = createTicketSet(request);  // for testing
  } else if (event == "getTicket") {
    response = getTicket(request);
  }

  else if (event == "") {
  }

  else {
    throw TException(500, "event request not found: " + event);
  }
}

void WsSession::do_read() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  ws.async_read(buffer,
                std::bind(&WsSession::on_read, shared_from_this(),
                          std::placeholders::_1, std::placeholders::_2));
}

void WsSession::on_read(boost::system::error_code ec, std::size_t size) {
  json::object response;
  std::optional<json::object> request;

  try {
    if (ec) {
      if (ec == ws::error::closed) {
        return;
      }
      throw TException(ec.value(), ec.message());
    }

    request = parse_message(size);
    handle_request(*request, response);

  } catch (TException& ex) {
    std::cout << "[x] Ex: " << ex.message();
    response = createResponse(*request, {}, ex.error_code(), ex.message());
  } catch (std::exception& ex) {
    if(!request)
      request = std::make_optional<boost::json::object>({{"event", "Invalid Json"}});
    std::cout << "[x] Ex: " << ex.what();
    response = createResponse(*request, {}, -1, ex.what());
  }

  do_write(jsonToString(response));

  do_read();
}

std::optional<json::object> WsSession::parse_message(size_t& size) {
  const std::string bufferStr = std::string((char*)buffer.data().data(), size);
  std::cout << "[<<] Message Recived: \n" << bufferStr << std::endl;
  std::optional<json::object> jobj;
  jobj = std::make_optional<json::object>(json::parse(bufferStr).as_object());
  buffer.clear();
  return jobj;
}

void WsSession::on_accept(boost::system::error_code ec) {
  if (ec) {
    std::cout << ec.message() << std::endl;
    return;
  }
  do_read();
}

void WsSession::do_write(const std::string& data) {
  std::cout << "[>>] SENDING: " << data << std::endl;
  ws.async_write(net::buffer(data, data.size()),
                 std::bind(&WsSession::on_write, shared_from_this(),
                           std::placeholders::_1, std::placeholders::_2));
}

void WsSession::do_write(json::object jobj) {
  do_write(jsonToString(jobj));
}

void WsSession::on_write(boost::system::error_code ec, std::size_t size) {
  if (ec)
    std::cout << "[e] Error in Writing Data";
  else
    std::cout << "[>>] Message sent, Size: " << size << std::endl;
}

const std::string WsSession::jsonToString(json::object jobj) {
  const std::string jstr = json::serialize(jobj);
  return jstr;
}

// TODO: request event name .c_str()
inline json::object WsSession::createResponse(json::object& request,
                                              json::object data,
                                              int16_t error_code,
                                              const std::string message) {
  return {{"type", "event"},
          {"event_type", "response"},
          {"event", request["event"].as_string()},
          {"data", data},
          {"error_code", error_code},
          {"message", message}};
}
