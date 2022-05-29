#include "WsSession.h"

json::object WsSession::createGameSession(json::object& request) {
  auto gs_db = handler->getDB()->addGameSession(currentUser()->board());
  //  auto gs = handler->gameSession()->create(gs_db, currentUser()->board());
  //  return createResponse(request, {"game_session_id", gs});
}

json::object WsSession::getGameSessionById(json::object& request) {}

json::object WsSession::getGameSessionByUid(json::object& request) {}

json::object WsSession::getGameSessionByTid(json::object& request) {}
