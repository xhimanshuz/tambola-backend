#ifndef TICKETHANDLER_H
#define TICKETHANDLER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include "Handler/Handler.h"
#include "Ticket/Ticket.h"
#include "Ticket/ticket_db.h"

class TicketHandler : public Handler {
  std::map<uint16_t, std::vector<std::shared_ptr<Ticket>>> newTicketSetMap;
  std::map<uint16_t, std::vector<std::shared_ptr<Ticket>>> usedTicketSetMap;

 public:
  TicketHandler(HandlerManager* parent);

  std::shared_ptr<Ticket> create(const std::shared_ptr<User>& user, const uint64_t& bid);
  std::shared_ptr<Ticket> create(std::shared_ptr<User> const& user, std::shared_ptr<GameSession> const& game_session);
  std::shared_ptr<Ticket> create(uint64_t const& uid_of_host, std::shared_ptr<User> const& user);

  std::shared_ptr<Ticket> getNewTicket(std::shared_ptr<User> const& user, std::shared_ptr<GameSession> const& game_session);
  std::vector<std::shared_ptr<Ticket>> createSet();
  std::shared_ptr<Ticket> getById_tsid(std::string const& tid, uint64_t const tsid);
  std::shared_ptr<Ticket> getById(std::string const& tid);
  std::shared_ptr<Ticket> getByUid(const uint64_t uid);
  bool removeTicketById(
      std::string const& id, uint64_t const sid,
      std::map<uint16_t, std::vector<std::shared_ptr<Ticket>>>& ticketSetMap);
  bool removeTicketById_tsid(
      std::string const& id, uint64_t const tsid);
  bool moveToUsed(std::shared_ptr<Ticket> ticket);
  uint64_t extractSetId(const std::string id);
};

#endif  // TICKETHANDLER_H
