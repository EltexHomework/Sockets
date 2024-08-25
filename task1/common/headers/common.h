#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#define CLIENTS_AMOUNT 5
#define BUFFER_SIZE 128
#define SOCK_PATH "./sock"
#define print_error(msg) do {perror(msg); \
  exit(EXIT_FAILURE);} while(0)

#endif // !COMMON_H
