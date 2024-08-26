#ifndef SERVER_H
#define SERVER_H

#include "../../common/headers/common.h"

/**
 * Used to create server on local adress family (AF_LOCAL) with
 * UDP protocol. 
 */
struct server {
  /* Address of the server */
  struct sockaddr_un serv;
  
  /* Socket file descriptor */
  int sfd;
};

struct server* create_server(const char* path);

void run_server(struct server* server);

void send_message(struct server* server, struct sockaddr_un* client, char buffer[BUFFER_SIZE]);
  
char* recv_message(struct server* server, struct sockaddr_un* client);

char* edit_message(char* message);

void close_connection(struct server* server);

void free_server(struct server* server);

#endif // !SERVER_H
