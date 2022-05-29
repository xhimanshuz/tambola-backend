#include "WsSession.h"

json::object WsSession::createTicket(json::object& request) {
  auto bid = request["bid"].as_int64();

  auto board = handler->board()->getById(bid);
  if (!board)
    throw TException(404, "Board id not found.");

  auto ticket = handler->ticket()->create(currentUser(), bid);
  board->getGameSession()->add(ticket);
  _currentUser->setTicket(ticket);
  return createResponse(request, ticket->toJson());
}

json::object WsSession::createTicketByBid(json::object& request) {

}

json::object WsSession::createTicketByGS(json::object& request) {}

json::object WsSession::createTicketSet(json::object& request) {}

json::object WsSession::getTicket(json::object& request) {
  return createResponse(request, _currentUser->ticket()->toJson());
}

json::object WsSession::getTicketById(json::object& request) {
  auto id = request["id"].as_string().c_str();
  return createResponse(request, handler->ticket()->getById(id)->toJson());
}

json::object WsSession::getTicketByUid(json::object& request) {
  auto uid = request["uid"].as_int64();
  auto ticket = handler->ticket()->getByUid(uid);
  return createResponse(request, ticket->toJson(), (ticket) ? 0 : 404,
                        (ticket) ? "" : "Ticket not found");
}
