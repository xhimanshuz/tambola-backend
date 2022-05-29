#include "board.h"
#include <numeric>
#include "GameSession/GameSession.h"
#include "TException.h"
#include "User/User.h"
#include "chrono"

Board::Board(std::shared_ptr<User>& user, uint64_t const& id)
    : TObject(id), UsersOwn(user) {
  setupBoard();
}

Board::Board(Board const& board)
    : TObject(board.getId()), UsersOwn(board.getUser())
{
  _numbersOnBoard = board.getNumbersOnBoard();
  _numbersOnBucket = board.getNumbersOnBucket();
  _gameSession = board.getGameSession();
}

// Board::Board(const Board& board)
//    : TObject{board.getId()}, UsersOwn{board.getUser()} {}

void Board::setGameSession(const std::shared_ptr<GameSession>& newGameSession) {
  _gameSession = newGameSession;
}

const std::shared_ptr<GameSession>& Board::getGameSession() const {
  return _gameSession;
}

// Board::Board(uint64_t id, std::shared_ptr<User> user)
//    :, _numbersOnBucket{90, 0}, _user{user} {
//  srand(time(0));
//  setupBoard();
//}

void Board::setupBoard() {
  _numbersOnBucket.resize(90);
  std::iota(_numbersOnBucket.begin(), _numbersOnBucket.end(), 1);
}

void Board::setId(u_int64_t id) {
  uint64_t* ns_id = const_cast<uint64_t*>(&_id);  // ns_id: non constant id
  *ns_id = id;
}

uint16_t Board::pickNumber() {
  if (_numbersOnBucket.empty())
    throw TException(300, "Bucket is empty.");
  uint16_t pickedNumber = _numbersOnBucket.at(rand() % _numbersOnBucket.size());
  _numbersOnBucket.erase(std::find(_numbersOnBucket.begin(),
                                   _numbersOnBucket.end(), pickedNumber));
  _numbersOnBoard.push_back(pickedNumber);
  //  insertNumberOnBoard(pickedNumber);
  std::cout << "[!] Picker Number: " << pickedNumber << std::endl;
  _gameSession->numberPicked(pickedNumber);
  return pickedNumber;
}

std::vector<uint16_t> Board::getNumbersOnBoard() const {
  return _numbersOnBoard;
}

std::vector<uint16_t> Board::getNumbersOnBoardSorted() {
  auto sorted_numberOnBoard = _numbersOnBoard;
  std::sort(sorted_numberOnBoard.begin(), sorted_numberOnBoard.end());
  return sorted_numberOnBoard;
}

std::vector<uint16_t> Board::getNumbersOnBucket() const {
  return _numbersOnBucket;
}

boost::json::array Board::vectorToJsonArray(std::vector<uint16_t> vectorData) {
  boost::json::array json_array;
  for (auto& data : vectorData)
    json_array.emplace_back(data);
  return json_array;
}

uint16_t Board::getLastPickedNumber() {
  if (getNumbersOnBoard().empty())
    return 0;
  return getNumbersOnBoard().back();
}

boost::json::object Board::toJson() {
  return {{"board_id", _id},
          {"numbers_on_board", getNumbersOnBoardSorted()},
          {"last_picked_number", getLastPickedNumber()},
          {"remaining_numbers", getNumbersOnBucket()},
          {"picked_numbers_in_sequence", getNumbersOnBoard()}};
}
