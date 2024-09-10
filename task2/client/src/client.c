#include "../headers/client.h"

/*
 * create_client - used to create an object of
 * client struct. 
 * @client_path - path to client socket file
 * @server_path - path to server socket file
 *
 * Return: pointer to an object of client struct
 */
struct client* create_client(const char* client_path, const char* server_path) {
  struct client* client = (struct client*) malloc(sizeof(struct client));
  if (!client)
    print_error("malloc");

  /* Initialzie server sockaddr_un struct*/
  client->serv.sun_family = AF_LOCAL;
  strncpy(client->serv.sun_path, server_path, sizeof(client->serv.sun_path) - 1);
  
  /* Initialzie client sockaddr_un struct*/
  client->client.sun_family = AF_LOCAL;
  unlink(client_path);
  strncpy(client->client.sun_path, client_path, sizeof(client->serv.sun_path) - 1);

  client->sfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (client->sfd == -1)
    print_error("socket");

  return client;
}

/* run_client - used to bind client addr
 * specified int client->client to socket.
 * @client - pointer to an object of client struct
 */
void run_client(struct client* client) {
  socklen_t client_size = sizeof(client->client);   
   
  if (bind(client->sfd, (struct sockaddr*) &client->client, client_size) == -1)
    print_error("bind");
  
  /* Process user input */
  process_input(client);
}

/*
 * process_input - used to receive user input
 * from stdin. Terminates received string, calls
 * send_message and waits for server response.
 * @client - pointer to an object of client struct
 */
void process_input(struct client* client) {
  char buffer[BUFFER_SIZE];
  
  /* Wait for user input */
  while (1) {
    printf("Enter message: ");
    
    /* Read user input */
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
      print_error("fgets");
    buffer[strlen(buffer) - 1] = '\0';

    /* Send user message */
    send_message(client, buffer);

    /* Receive answer */
    char* message = recv_message(client);
    if (message == NULL) {
      close_connection(client);
      break;
    }

    printf("SERVER: Server %s send response: %s\n", client->serv.sun_path, message);
    free(message);
  }
}

/*
 * send_message - used to send message to server.  
 * @client - pointer to an object of client struct 
 * @buffer - message
 */
void send_message(struct client* client, char buffer[BUFFER_SIZE]) {
  ssize_t bytes_send;
  socklen_t serv_len = sizeof(client->serv);

  bytes_send = sendto(client->sfd, buffer, strlen(buffer), 0, (struct sockaddr*) &(client->serv), serv_len);

  if (bytes_send == -1)
    print_error("sendto");

  printf("CLIENT: Send message to %s: %s\n", client->serv.sun_path, buffer);
}

/*
 * recv_message - used to receive message from server.
 * Allocates memory for buffer which should be freed manually.
 *
 * Return: string (message) if successful, NULL if connection terminated
 */
char* recv_message(struct client* client) {
  ssize_t bytes_read;
  socklen_t serv_len;
  char* buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
  
  serv_len = sizeof(client->serv);
  bytes_read = recvfrom(client->sfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &client->serv, &serv_len);
  
  /* Truncate buffer */
  buffer[bytes_read] = '\0';

  if (bytes_read == -1)
    print_error("recvfrom");
  else if (bytes_read == 0)
    return NULL;

  return buffer;
}

/*
 * close_connection - used to close connection with close
 * call.
 * @client - pointer to an object of client struct
 */
void close_connection(struct client* client) {
  close(client->sfd);
}

/*
 * free_client - used to free allocated memory for client
 * object.
 * @client - pointer to an object of client struct
 */
void free_client(struct client* client) {
  free(client);
}
