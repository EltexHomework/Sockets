#ifndef CLIENT_H
#define CLIENT_H

#include "../../common/headers/common.h"

/*
 * Used as client for connection to local address
 * family (AF_LOCAL) server via UDP protocol. 
 */
struct client {
  /* Address of the server */
  struct sockaddr_un serv;

  /* Address of the client*/
  struct sockaddr_un client;

  /* Server file descriptor*/
  int sfd;
};

struct client* create_client(const char* client_path, const char* server_path);

void run_client(struct client* client);

void process_input(struct client* client);

void send_message(struct client* client, char buffer[BUFFER_SIZE]);

char* recv_message(struct client* client);

void close_connection(struct client* client);

void free_client(struct client* client);

#endif // !CLIENT_H
