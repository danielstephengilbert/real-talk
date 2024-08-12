#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/get_listener.h"
#include "../include/run_server.h"

typedef struct pollfd connection;

/**
 * Facilitate connections and communication between them.
 */
int main(void) {

  // Connection data structures.
  connection* conns;

  // Connection-related data.
  int conn_count;
  int max_conns;
  int conns_size;

  // Listener variables.
  int listener_fd;
  int listener_error;

  // Multi-process stuff.
  int server_pid;

  // Allocate memory for connections.
  max_conns   = 5;
  conns_size  = sizeof(connection) * max_conns;
  conns       = malloc(conns_size);

  // Get a listener to receive connections on the server.
  listener_fd     = get_listener();
  listener_error  = (listener_fd == -1);

  // Throw error if cannot get listener.
  if (listener_error) {
    fprintf(stderr, "Error: Could not start listener.\n");
    exit(1);
  }

  // Make the listener the first connection on the server.
  conns[0].fd      = listener_fd;
  conns[0].events  = POLLIN;
  conn_count       = 1;

  // server_pid = fork();
  // Add conditional logic for server/client code.
  run_server(&conns, &conn_count, &max_conns, listener_fd);

  return 0;

}
