#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct addrinfo addr_info;

/**
 * Establish a listener to receive incoming connections on.
 */
int get_listener(void) {

  // Port information.
  char* port = "55555";

  // IP address and connection-related info used to
  // obtain a socket for communicating that way.
  addr_info addr_spec;

  // Variables related to the getaddrinfo function.
  int getaddrinfo_val;
  int getaddrinfo_error;

  // IP addresses to iterate over until a valid one is found.
  addr_info* first_addr;
  addr_info* addr_iter;

  // Listener-related variables.
  int listener_fd;
  int listener_invalid;
  int listener_proto_level = 1;

  // Bind function related variables.
  int bind_val;
  int bind_error;
  int bind_failed;

  // Listen function related variables.
  int listen_val;
  int listen_error;

  // Zero out the address specification memory.
  memset(&addr_spec, 0, sizeof(addr_spec));

  // Specify the type of address and connection to use.
  addr_spec.ai_family   = AF_UNSPEC;
  addr_spec.ai_socktype = SOCK_STREAM;
  addr_spec.ai_flags    = AI_PASSIVE;

  // Populate the address info with the first address found.
  getaddrinfo_val   = getaddrinfo(NULL, port, &addr_spec, &first_addr);
  getaddrinfo_error = (getaddrinfo_val != 0);

  // Throw error if could not get address.
  if (getaddrinfo_error) {
    fprintf(stderr, "Error: %s\n", gai_strerror(getaddrinfo_val));
    exit(1);
  }

  // Search for a valid IP address that can be used to establish a listener.
  for (
    addr_iter  =  first_addr;
    addr_iter  != NULL;
    addr_iter  =  addr_iter->ai_next
  ) {
    
    // Try to create a socket.
    listener_fd = socket(
      addr_iter->ai_family, addr_iter->ai_socktype, addr_iter->ai_protocol
    );
    listener_invalid = (listener_fd < 0);
    // If failure, try with the next available address.
    if (listener_invalid) continue;

    // Set the socket options.
    setsockopt(
      listener_fd, SOL_SOCKET, SO_REUSEADDR, &listener_proto_level, sizeof(int)
    );

    // Try to bind the socket.
    bind_val    = bind(listener_fd, addr_iter->ai_addr, addr_iter->ai_addrlen);
    bind_error  = (bind_val < 0);
    // If failure, try with the next available address.
    if (bind_error) {
      close(listener_fd);
      continue;
    }

    break;

  }

  // Deallocate memory used for address.
  freeaddrinfo(first_addr);

  // Throw error if bind failed for all available addresses.
  bind_failed = (addr_iter == NULL);
  if (bind_failed) return -1;

  // Try to listen on the socket, and throw error upon failure.
  listen_val    = listen(listener_fd, 10);
  listen_error  = (listen_val == -1);
  if (listen_error) return -1;

  // Return the listener if successful.
  return listener_fd;

}
