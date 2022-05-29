#include "HttpSession.h"

HttpSession::HttpSession(boost::beast::tcp_stream _stream,
                         http::request<http::string_body> _req)
    : stream{std::move(_stream)}, req{std::move(_req)} {}

void HttpSession::start() { handle_request(); }

void HttpSession::handle_request() {
  if (req.method() == http::verb::get) {
    get_request();
  } else if (req.method() == http::verb::post) {
    post_request();
  }

  send_response();
}

void HttpSession::get_request() {
  std::cout << "[!] GET REQUEST RECEIVED." << std::endl;
  if (req.target() == "test_get")
    std::cout << "Test GET Request." << std::endl;
  else
    std::cout << "Invalid Get Request." << std::endl;
}

void HttpSession::post_request() {
  std::cout << "[!] POST REQUEST RECEIVED." << std::endl;
  if (req.target() == "test_get")
    std::cout << "Test POST Request." << std::endl;
  else
    std::cout << "Invalid POST Request." << std::endl;
}

json::object HttpSession::parse_json() {
  json::object jObj;
  auto body = req.body();
  jObj = json::parse(req.body().data()).get_object();

  std::cout << "[!] Json: " << jObj << std::endl;
  return jObj;
}

void HttpSession::send_response() {
  http::response<http::string_body> resp{http::status::ok, req.version()};
  json::object jobj{{"status", "ok"}};
  resp.set(http::field::content_type, "application/json");
  resp.body() = json::serialize(jobj).c_str();
  resp.prepare_payload();

  if (http::write(stream, resp, ec); ec) {
    std::cout << "Error in Writing: " << ec.message() << std::endl;
  }

  stream.close();
}
