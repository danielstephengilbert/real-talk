#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/process_client.h"
#include "../include/process_listener.h"

typedef struct pollfd connection;

void run_server(
  connection**  conns,
  int*          conn_count,
  int*          max_conns,
  int           listener_fd
) {

  // Connection data structure.
  connection conn;

  // Connection-related data.
  int conn_index;

  // Poll variables.
  int poll_val;
  int poll_error;

  // Data ready flag info.
  int data_ready;
  int listener_data;
  int client_data;

  // Infinite loop to keep server running until terminated.
  while (1) {

    // Retrieve connection information for all connections
    // to see if any of them have data ready.
    poll_val    = poll(*conns, *conn_count, -1);
    poll_error  = (poll_val == -1);

    // Throw error if could not retrieve connection information.
    if (poll_error) {
      perror("poll");
      exit(1);
    }

    // Check every connection to see if it has data ready.
    for (conn_index = 0; conn_index < *conn_count; conn_index++) {

      // Get the current connection.
      conn = (*conns)[conn_index];

      // Check if data ready, and from which type of connection.
      data_ready    = (conn.revents & POLLIN);
      listener_data = (conn.fd == listener_fd);
      client_data   = !listener_data;

      // If data is ready from the listener connection, process it.
      if (data_ready && listener_data) {
        process_listener(listener_fd, conns, conn_count, max_conns);
        
      // If data is ready from a client connection, process it.
      } else if (data_ready && client_data) {
        process_client(conn_index, conns, conn_count, listener_fd);
        
      }

    }

  }

}
