
#include <stdio.h>
#include <string.h>

void printUsage();

int main(int argc, char *argv[]) {

  if (argc != 4) {
    printUsage();
    return 1;
  }

  char *mode = argv[1];
  char *ip   = argv[2];
  char *port = argv[3];

  int server_mode = strcmp(mode, "-s") == 0;
  int client_mode = strcmp(mode, "-c") == 0;

  if (server_mode) {
    printf("server %s %s\n\n", ip, port);
  } else if (client_mode) {
    printf("client %s %s\n\n", ip, port);
  } else {
    printUsage();
    return 1;
  }

  return 0;

}

void printUsage() {
  printf("\nUsage:\n");
  printf("server: rt -s server_ip server_port\n");
  printf("client: rt -c server_ip server_port\n\n");
}
