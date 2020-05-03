#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <condition_variable>

#include "BrainEngine.h"
#include "Session.h"

class Server
{
    std::shared_ptr<boost::asio::io_context> ioc;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    std::shared_ptr<std::vector<std::shared_ptr<Session>>> sessionVector;

    std::shared_ptr<BrainEngine> brain;
    int noOfSession;
    bool ready;

    void disconnectAll();
public:
    Server(std::shared_ptr<boost::asio::io_context> _ioc);
    void acceptConnection();


};


#endif // SERVER_H
