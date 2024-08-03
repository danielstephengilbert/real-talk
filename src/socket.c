#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_listener_socket(void) {

  // Init all vars
  int yes       = 1;
  int status    = 0;
  int listener  = 0;
  
  // Port
  // Use autoincrementing logic to find a valid
  // port for the user... (TODO)
  char *port = "55555";

  struct addrinfo  addr_spec;
  struct addrinfo *first_addr;
  struct addrinfo *addr;

  // Zero out the memory pointed to by addr_spec
  memset(&addr_spec, 0, sizeof addr_spec);

  // Set the desired address type info
  addr_spec.ai_family   = AF_UNSPEC;
  addr_spec.ai_socktype = SOCK_STREAM;
  addr_spec.ai_flags    = AI_PASSIVE;

  // Use the desired address type info (hints)
  // to populate first_addr with valid addrinfo data
  status = getaddrinfo(NULL, port, &addr_spec, &first_addr);
  if (status != 0) {
    fprintf(stderr, "Error: %s\n", gai_strerror(status));
    exit(1);
  }

  // Attempt to obtain and bind a socket file descriptor
  // using all possible addresses
  for (addr = first_addr; addr != NULL; addr = addr->ai_next) {
    
    // Get socket file descriptor
    listener = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (listener < 0) continue;

    // Set options for the file descriptor
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    // Bind the socket to the address
    status = bind(listener, addr->ai_addr, addr->ai_addrlen);
    if (status < 0) {
      close(listener);
      continue;
    }

    // Exit loop if socket successfully created and bound
    break;

  }

  freeaddrinfo(first_addr);

  // If the address is null, the bind failed
  if (addr == NULL) {
    return -1;
  }

  // Listen on socket address
  status = listen(listener, 10);
  if (status == -1) {
    return -1;
  }

  return listener;

}
