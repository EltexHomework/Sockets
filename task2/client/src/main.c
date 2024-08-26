#include "../headers/client.h"
#include <sys/socket.h>

struct client* client;

void cleanup();

int main(void) {
  client = create_client(CLIENT_SOCK_PATH, SERV_SOCK_PATH);
  atexit(cleanup);
  run_client(client);
  exit(EXIT_SUCCESS);
}

void cleanup() {
  close_connection(client);
  free_client(client);
}
