#ifndef BOARD_H
#define BOARD_H

#include "TObject.h"
#include "User/UsersOwn.h"

#include <stdint.h>
#include <time.h>
#include <algorithm>
#include <boost/json.hpp>
#include <iostream>
#include <map>
#include <vector>

class User;
class GameSession;

class Board : public TObject, public UsersOwn {
  std::vector<uint16_t> _numbersOnBoard;
  std::vector<uint16_t> _numbersOnBucket;
  std::shared_ptr<GameSession> _gameSession;

 public:
  Board(std::shared_ptr<User>& user, uint64_t const& id = 0);
  Board(Board const& board);
  //  Board(Board const& board);

  /**
   * @brief Setup Board number, generate
   * number in Buckets (1-90).
   */
  void setupBoard();

  void setId(u_int64_t id);

  /**
   * Pick number from Board.
   *
   * @brief Pick the number at random position.
   * @return picked number.
   */
  uint16_t pickNumber();

  /**
   * @brief Get all available number on
   * board.
   * @return list of number from board.
   */
  std::vector<uint16_t> getNumbersOnBoard() const;

  /**
   * @brief Get all picked number in sorted.
   * @return list of all picked numbers in sorted.
   */
  std::vector<uint16_t> getNumbersOnBoardSorted();

  /**
   * @brief Get all unpicked number.
   * @return list of all unpicked number.
   */
  std::vector<uint16_t> getNumbersOnBucket() const;

  boost::json::array vectorToJsonArray(std::vector<uint16_t> vectorData);
  uint16_t getLastPickedNumber();
  void insertNumberOnBoard(u_int16_t number);

  boost::json::object toJson() override;
  void setGameSession(const std::shared_ptr<GameSession>& newGameSession);
  const std::shared_ptr<GameSession>& getGameSession() const;
};

#endif  // BOARD_H
