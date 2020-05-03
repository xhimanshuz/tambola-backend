#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

class Session/*: public std::enable_shared_from_this<Session>*/
{
    std::string remoteAddress;
    uint32_t remotePort;
    bool validConection;
    std::string username;

    void eventLoop();

public:
    boost::asio::ip::tcp::socket socket;
    uint32_t uid;
    Session(boost::asio::ip::tcp::socket socket, uint32_t _uid);
    ~Session();

    std::string readData();
    void writeData(const std::string &data);

    std::string readJson();
    void writeJson(const std::string &data);

};

#endif // SESSION_H
