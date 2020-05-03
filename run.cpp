#include <boost/asio/io_context.hpp>

#include "Server.h"
#include "BrainEngine.h"

int main()
{
    auto ioc = std::make_shared<boost::asio::io_context>();

    Server s(ioc);

    ioc->run();

//    BrainEngine b;

    return 0;
}
