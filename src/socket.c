#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int create_socket() {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    fprintf(stderr, "\nFailed to create socket.\n");
    exit(1);
  }

  return sockfd;

}

void destroy_socket(int sockfd) {

  int status = close(sockfd);

  if (status != 0) {
    fprintf(stderr, "\nFailed to close socket.\n");
    exit(1);
  }

}
