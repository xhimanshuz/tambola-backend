#include <boost/asio/io_context.hpp>
#include <iostream>

#include "listener.h"

int main(int argc, char** argv) {
  boost::asio::io_context ioc;
  Listener l{ioc};
  ioc.run();

  return EXIT_SUCCESS;
}
