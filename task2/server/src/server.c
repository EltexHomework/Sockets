#include "../headers/server.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * create_server - used to create an object of server
 * struct, initializes its fields.
 * @path - path to socket file
 *
 * Return: pointer to an object of server struct 
 */
struct server* create_server(const char* path) {
  struct server* server = (struct server*) malloc(sizeof(struct server));
  if (!server)
    print_error("malloc");

  /* Initialzie sockaddr_un struct */
  server->serv.sun_family = AF_LOCAL;
  unlink(path);
  strncpy(server->serv.sun_path, path, sizeof(server->serv.sun_path) - 1);
  
  /* Create a socket */
  server->sfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (server->sfd == -1)
    print_error("socket");

  return server;
}

/*
 * run_server - used to bind server, set it
 * to passive mode and accept connections
 * @server - pointer to an object of server struct
 */
void run_server(struct server* server) {
  struct sockaddr_un client;
  socklen_t client_size = sizeof(client);

  /* Bind Endpoint to socket */
  if (bind(server->sfd, (struct sockaddr*) &server->serv, sizeof(server->serv)) == -1)
    print_error("bind");
    
  printf("SERVER: Server %s started\n", server->serv.sun_path);

  /* Wait for data */
  while (1) {
    char* buffer = recv_message(server, &client);
    char* reply = edit_message(buffer);
    
    printf("SERVER: Received message from %s: %s\n", client.sun_path, buffer);
    send_message(server, &client, reply);

    free(buffer);
    free(reply);
  }
}

/*
 * send_message - used to send message to client.
 * @server - pointer to an object of server struct
 * @client - address of client (sockaddr_un)
 * @buffer - message
 */
void send_message(struct server* server, struct sockaddr_un* client, char buffer[BUFFER_SIZE]) {
  ssize_t bytes_send;
  socklen_t client_len = sizeof(*client);

  bytes_send = sendto(server->sfd, buffer, strlen(buffer), 0, (struct sockaddr*) client, client_len);

  if (bytes_send == -1)
    print_error("sendto");

  printf("SERVER: Send message to %s: %s\n", client->sun_path, buffer);
}

/*
 * recv_message - used to receive message from server.
 * allocates memory for message, then receives it all. Allocated
 * buffer must be freed manually.
 * @server - pointer to an object of server struct
 * @client - address of client (sockaddr_un)
 *
 * Return: string (message) if successful, NULL if connection terminated
 */
char* recv_message(struct server* server, struct sockaddr_un* client) {
  ssize_t bytes_read;
  socklen_t client_len;
  char* buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
  
  client_len = sizeof(*client);

  bytes_read = recvfrom(server->sfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) client, &client_len);  
  
  /* Truncate buffer */
  buffer[bytes_read] = '\0';

  if (bytes_read == -1)
    print_error("recvfrom");
  else if (bytes_read == 0)
    return NULL;

  return buffer;
}

/*
 * edit_message - used to add prefix "Server" to message.
 * Allocates new buffer. Return buffer should be free manually.
 * @message - message from client that needs to be changed
 * 
 * Return: string with prefix
 */
char* edit_message(char* message) {
  char* prefix = "Server";
  char* new_message = (char*) malloc(strlen(message) + strlen(prefix) + 2);
  if (!new_message)
    print_error("malloc");

  /* Add prefix to message */
  snprintf(new_message, strlen(message) + strlen(prefix) + 2, "%s %s", "Server", message);

  return new_message;
}

/*
 * close_connection - used to close connection when client
 * called shutdown. Closes clients file descriptor and
 * frees memory allocated for client; 
 */
void close_connection(struct server* server) {
  close(server->sfd);
}

/*
 * free_server - free allocated memory for server 
 * @server - pointer to an object of server struct
 */
void free_server(struct server* server) {
  free(server);
}
