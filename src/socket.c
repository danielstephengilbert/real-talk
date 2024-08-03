#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int get_listening_socket(char *port) {

  struct addrinfo addr_spec, *first_addr, *addr;

  memset(&addr_spec, 0, sizeof addr_spec);

  addr_spec.ai_family   = AF_UNSPEC;
  addr_spec.ai_socktype = SOCK_STREAM;
  addr_spec.ai_flags    = AI_PASSIVE;

  int status = getaddrinfo(NULL, port, &addr_spec, &first_addr);
  if (status != 0) {
    fprintf(stderr, "\nCould not get address info.\n");
    exit(1);
  }

  int listener = 0;
  int yes = 1;

  for (addr = first_addr; addr != NULL; addr = addr->ai_next) {

    listener = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (listener < 0) continue;

    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    status = bind(listener, addr->ai_addr, addr->ai_addrlen);
    if (status < 0) {
      close(listener);
      continue;
    }

    break;

  }

  freeaddrinfo(first_addr);

  if (addr == NULL) {
    fprintf(stderr, "\nFailed to bind.\n");
    return -1;
  }

  status = listen(listener, 10);
  if (status == -1) {
    fprintf(stderr, "\nFailed to listen.\n");
    return -1;
  }

  return listener;

}
