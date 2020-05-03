#include "Server.h"
using namespace std::literals::chrono_literals;

Server::Server(std::shared_ptr<boost::asio::io_context> _ioc): ioc(_ioc), ready(false), noOfSession{0}
{
    acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8000), true);
    sessionVector = std::make_shared<std::vector<std::shared_ptr<Session>>>();
    brain = std::make_shared<BrainEngine>();

    std::thread([this]{
        while(true)
        {
            if(sessionVector->size() < 5)
            {
                std::this_thread::sleep_for(10s);
                continue;
            }
            auto numberBroadCast = std::to_string(brain->pickNumber());
            if(numberBroadCast == "0")
                disconnectAll();
            for(auto i: *sessionVector)
            {
//                std::cout<<"Writing to Session\n";
                try
                {
                    i->writeData(numberBroadCast+' ');
                }
                catch (boost::system::system_error se)
                {
                    std::cout<< "Writing Error: "<< se.what()<<" FOR SOCKET: "<< i->uid<<std::endl;
                    if(i)
                        i.reset();
                    sessionVector->erase(std::find(sessionVector->begin(), sessionVector->end(), i));
                }
            }
            std::this_thread::sleep_for(1s);
        }
    }).detach();
    acceptConnection();
}

void Server::acceptConnection()
{
    std::cout<<"WAITING TO ACCEPT CONNECTION"<<std::endl;
    acceptor->async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        try
        {
            if(!ec)
            {
                auto s = std::make_shared<Session>(std::move(socket), ++noOfSession);
                sessionVector->push_back(s);
                s->writeData("YOUR SOCKET NAME IS: "+ std::to_string(s->uid)+'\n');
                for(auto i: *brain->pickedNumber)
                    s->writeData(std::to_string(i)+' ');
            }
            else
                std::cout<<"Error Code: "<< ec.message()<<std::endl;
        }
        catch(boost::system::system_error &ec)
        {
            std::cout<<"Error Code: "<< ec.what();
        }

        acceptConnection();
    });
    //    acceptConnection();
}

void Server::disconnectAll()
{
    for(auto& session: *sessionVector)
    {
        session.reset();
    }
    sessionVector->clear();
}
