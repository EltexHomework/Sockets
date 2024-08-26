#include "../headers/server.h"

struct server* server;

void cleanup();

int main(void) {
  server = create_server(SERV_SOCK_PATH);
  atexit(cleanup);
  run_server(server); 
  exit(EXIT_SUCCESS);
}

void cleanup() {
  close_connection(server);
  free_server(server); 
}
