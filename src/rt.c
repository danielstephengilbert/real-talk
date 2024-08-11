#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/get_listener.h"
#include "../include/process_client.h"
#include "../include/process_listener.h"
#include "../include/remove_connection.h"

typedef struct addrinfo  addr_info;
typedef struct pollfd    connection;
typedef struct sockaddr  address;

/**
 * Facilitate connections and communication between them.
 */
int main(void) {

  // Connection data structures.
  connection* conns;
  connection* conn;

  // Connection-related data.
  int conn_index;
  int conn_count;
  int max_conns;
  int conns_size;

  // Listener variables.
  int listener_fd;
  int listener_error;

  // Poll variables.
  int poll_val;
  int poll_error;

  // Data ready flag info.
  int data_ready;
  int listener_data;
  int client_data;

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

  // Infinite loop to keep server running until terminated.
  while (1) {

    // Retrieve connection information for all connections
    // to see if any of them have data ready.
    poll_val    = poll(conns, conn_count, -1);
    poll_error  = (poll_val == -1);

    // Throw error if could not retrive connection information.
    if (poll_error) {
      perror("poll");
      exit(1);
    }

    // Check every connection to see if it has data ready.
    for (conn_index = 0; conn_index < conn_count; conn_index++) {

      // Get the current connection.
      conn = &conns[conn_index];
      
      // Check if data ready, and from which type of connection.
      data_ready    = (conn->revents & POLLIN);
      listener_data = (conn->fd == listener_fd);
      client_data   = !listener_data;

      // If data is ready from the listener connection, process it.
      if (data_ready && listener_data) {
        process_listener(listener_fd, &conns, &conn_count, &max_conns);
      // If data is ready from a client connection, process it.
      } else if (data_ready && client_data) {
        process_client(conn_index, &conns, &conn_count, listener_fd);
      }

    }

  }

  return 0;

}
