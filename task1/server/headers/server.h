#ifndef SERVER_H
#define SERVER_H

#include "../../common/headers/common.h"
#include "client.h"

/**
 * Used to create server on local adress family (AF_LOCAL) with
 * TCP protocol. 
 */
struct server {
  /* Address of the server */
  struct sockaddr_un serv;
  
  /* Array of pointers to clients */
  struct client** clients;
  int clients_amount;

  /* Passive socket to accept connecitons */
  int sfd;
};

struct server* create_server(const char* path);

void run_server(struct server* server);

void* handle_client_connection(void* arg);

void add_client(struct server* server, struct sockaddr_un* client, int client_fd);

void delete_client(struct server* server, struct client* client);

void send_message(struct client* client, char buffer[BUFFER_SIZE]);

char* recv_message(struct client* client);

char* edit_message(char* message);

void close_connection(struct client *client);

#endif // !SERVER_H
