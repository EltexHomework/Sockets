#include "../headers/server.h"

struct server* server;

void cleanup();

int main(void) {
  server = create_server(SOCK_PATH);
  atexit(cleanup);
  run_server(server); 
  exit(EXIT_SUCCESS);
}

void cleanup() {
  
}
