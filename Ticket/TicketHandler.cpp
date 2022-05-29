#include "TicketHandler.h"
#include "Handler/HandlerManager.h"
#include "TException.h"

TicketHandler::TicketHandler(HandlerManager* parent)
    : Handler(parent) {}

std::shared_ptr<Ticket> TicketHandler::create(const std::shared_ptr<User>& user,
                                              const uint64_t& bid) {
  auto board = parent()->board()->getById(bid);
  if(!board)
    throw TException(404, "Board not found with id: " + std::to_string(bid));
  auto ticket = getNewTicket(user, board->getGameSession());
  parent()->getDB()->addTicket(ticket);
  return ticket;
}

std::shared_ptr<Ticket> TicketHandler::create(const std::shared_ptr<User> &user, const std::shared_ptr<GameSession> &game_session)
{
  return getNewTicket(user, game_session);
}

std::shared_ptr<Ticket> TicketHandler::create(const uint64_t &uid_of_host, const std::shared_ptr<User> &user)
{
  auto other_host = parent()->user()->getByIDFromOnline(uid_of_host);
  if(!other_host)
    throw TException(400, "Online User not found: " + std::to_string(uid_of_host));
  return getNewTicket(user, other_host->getGameSession());
}

std::shared_ptr<Ticket> TicketHandler::getNewTicket(std::shared_ptr<User> const& user, std::shared_ptr<GameSession> const& game_session)
{
  if (newTicketSetMap.empty())
    createSet();
  auto ticket = newTicketSetMap.begin()->second.back();
  ticket->setOwner(user);
  ticket->setGameSession(game_session);
  moveToUsed(ticket);
  return ticket;
}

std::vector<std::shared_ptr<Ticket>> TicketHandler::createSet() {
  auto setId = parent()->getDB()->getNewTicketSetId();
  std::vector<std::shared_ptr<Ticket>> ticketSet;
  for (const auto& id : {0, 1, 2, 3, 4, 5})
    ticketSet.push_back(std::make_shared<Ticket>(setId, id));
  newTicketSetMap[setId] = ticketSet;
  return ticketSet;
}

std::shared_ptr<Ticket> TicketHandler::getById_tsid(std::string const& tid, uint64_t const tsid) {
  auto ticketSet = usedTicketSetMap[tsid];
  if (auto itr = std::find_if(ticketSet.begin(), ticketSet.end(),
                              [&tid](const std::shared_ptr<Ticket>& ticket) {
                                return ticket->getId() == tid;
                              });
      itr != ticketSet.end())
    return *itr;
  throw TException(404, "Ticket not foun with id: " + tid);
}

std::shared_ptr<Ticket> TicketHandler::getById(const std::string &tid)
{
  return getById_tsid(tid, extractSetId(tid));
}

std::shared_ptr<Ticket> TicketHandler::getByUid(const uint64_t uid) {
  for (auto& ticketSet : usedTicketSetMap) {
    if (auto itr = std::find_if(
            ticketSet.second.begin(), ticketSet.second.end(),
            [&](const std::shared_ptr<Ticket>& ticket) {
              return (ticket->getUser())
                         ? (ticket->getUser()->getId() == uid) ? true : false
                         : false;
            });
        itr != ticketSet.second.end())
      return *itr;
  }
  throw TException(404, "Ticket not foun with uid: " + std::to_string(uid));
}

bool TicketHandler::removeTicketById(
    std::string const& id,  uint64_t const sid,
    std::map<uint16_t, std::vector<std::shared_ptr<Ticket>>>& ticketSetMap) {
  if (auto itr = ticketSetMap.find(sid); itr != ticketSetMap.end()) {
    std::remove_if(itr->second.begin(), itr->second.end(),
                   [&id](const std::shared_ptr<Ticket>& ticket) {
                     return ticket->getId() == id;
                   });
    return true;
  }
  throw TException(300, "Error in removing the ticket while moving.");
}

bool TicketHandler::removeTicketById_tsid(std::string const& id, const uint64_t tsid)
{

}

bool TicketHandler::moveToUsed(std::shared_ptr<Ticket> ticket) {
  newTicketSetMap[ticket->getSetId()].pop_back();
  if (auto itr = usedTicketSetMap.find(ticket->getSetId());
      itr == usedTicketSetMap.end())
    usedTicketSetMap[ticket->getSetId()] = {};
  usedTicketSetMap[ticket->getSetId()].push_back(ticket);
  return true;
}

uint64_t TicketHandler::extractSetId(const std::string id) {
  return std::stoi(id.substr(0, id.size()-1));
}
