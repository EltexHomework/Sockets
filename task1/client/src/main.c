#include "../headers/client.h"
#include <sys/socket.h>

struct client* client;

void cleanup();

int main(void) {
  client = create_client(SOCK_PATH);
  atexit(cleanup);
  run_client(client);
  exit(EXIT_SUCCESS);
}

void cleanup() {
  shutdown_connection(client);
  free_client(client);
}
