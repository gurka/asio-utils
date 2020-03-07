#include <cstdio>
#include <asio.hpp>
#include "acceptor.h"

void accept_handler(asio::ip::tcp::socket&& socket)
{
  puts("Connected!");
  socket.close();
}

int main()
{
  asio::io_context io;

  // Create and start acceptor
  asioutils::Acceptor acceptor{&io, 5123, accept_handler};
  acceptor.start();

  // Create signal handler
  asio::signal_set signals{io, SIGINT, SIGTERM};
  signals.async_wait([&acceptor](const std::error_code&, int)
  {
    puts("Interrupted, stopping acceptor...");
    acceptor.stop();
  });

  // Run io context
  puts("Starting io_context");
  io.run();
  puts("io_context stopped");

  return 0;
}
