#include "WsSession.h"

json::object WsSession::createBoard(json::object& request) {
  auto board = handler->board()->create(
      currentUser(), handler->getDB()->addBoard(currentUser()));
  if (!board)
    throw TException(500, "Error in creating Board");
  handler->gameSession()->create(board);
  currentUser()->setBoard(board);
  handler->getDB()->updateBoard(board);
  return createResponse(request, {{"board_id", board->getId()}});
}

json::object WsSession::getBoardStats(json::object& request) {
  return createResponse(request, currentUser()->board()->toJson());
}

json::object WsSession::pickNumber(json::object& request) {
  auto response = createResponse(
      request, {{"picked_number", currentUser()->board()->pickNumber()}});
  handler->getDB()->updateBoard(currentUser()->board());
  return response;
}

json::object WsSession::getBoardById(json::object& request) {
  auto id = request["id"].as_int64();
  return createResponse(request, handler->board()->getById(id)->toJson());
}

json::object WsSession::getBoardByUid(json::object& request) {
  auto uid = request["uid"].as_int64();
  return createResponse(request, handler->board()->getByUId(uid)->toJson());
}

json::object WsSession::getBoard(json::object& request) {
  return createResponse(request, currentUser()->board()->toJson());
}
