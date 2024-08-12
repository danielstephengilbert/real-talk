#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

  // Fork process vars.
  int server_pid;
  int is_parent_process;
  int is_child_process;

  // Allocate memory for connections.
  max_conns   = 5;
  conns_size  = sizeof(connection) * max_conns;
  conns       = malloc(conns_size);

  // Get a listener to receive connections on the server.
  listener_fd     = get_listener();
  listener_error  = (listener_fd == -1);

  // Throw error if could not get listener.
  if (listener_error) {
    fprintf(stderr, "Error: Could not start listener.\n");
    exit(1);
  }

  // Make the listener the first connection on the server.
  conns[0].fd      = listener_fd;
  conns[0].events  = POLLIN;
  conn_count       = 1;

  // Fork process to handle server and client simultaneously.
  server_pid = fork();
  is_parent_process = (server_pid != 0);
  is_child_process  = !is_parent_process;

  // Run server using parent process.
  if (is_parent_process) {

    run_server(&conns, &conn_count, &max_conns, listener_fd);
  
  // Open host client in child process.
  } else {

    while (1) {  }

  }

  return 0;

}
