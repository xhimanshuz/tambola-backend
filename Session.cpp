#include "Session.h"
using namespace std::literals::chrono_literals;

Session::Session(boost::asio::ip::tcp::socket _socket, uint32_t _uid): socket(std::move(_socket)), uid(_uid)
{
    std::cout<<"SESSION CONNECTION FOR ID: "<<uid<<" "<< socket.remote_endpoint().address().to_string()<<":"<<socket.remote_endpoint().port()<<std::endl;
//    std::thread(&Session::eventLoop, this).detach();
}

Session::~Session()
{
    std::cout<<"SOCKET "<< uid<<" CLOSED"<<std::endl;
    socket.close();
}

void Session::eventLoop()
{

}

std::string Session::readData()
{
    std::vector<char> buff(10);
    socket.read_some(boost::asio::buffer(buff, buff.size()));
    int sizeToRead = std::stoi(buff.data());
    std::cout<< "Size To READ: "<< sizeToRead <<std::endl;
    buff.resize(sizeToRead);
    socket.read_some(boost::asio::buffer(buff, buff.size()));
    std::cout<< "Data Received: "<<buff.data()<<std::endl;
    buff.clear();
    return buff.data();
}

void Session::writeData(const std::string &data)
{
//    std::cout<< "Data Written: "<< data <<std::endl;

    socket.write_some(boost::asio::buffer(data.c_str(), data.size()));

}

std::string Session::readJson()
{

}

void Session::writeJson(const std::string &data)
{

}

