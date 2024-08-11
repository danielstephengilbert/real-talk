#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/get_listener.h"
#include "../include/process_listener.h"
#include "../include/remove_connection.h"

typedef struct addrinfo  addr_info;
typedef struct pollfd    connection;
typedef struct sockaddr  address;

void process_client(
  int           conn_index,
  connection**  conns,
  int*          conn_count,
  int           listener_fd
);

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

/**
 *  Process the client connection.
 */
void process_client(
  int           conn_index,
  connection**  conns,
  int*          conn_count,
  int           listener_fd
) {

  // Sender vars.
  connection  sender_conn;
  int         sender_fd;

  // Buffer vars.
  int   buf_len = 256;
  char  buffer[buf_len];

  // Data information.
  int data_len;
  int recv_error;
  int disconnected;

  // Connection info.
  int client_fd;
  int not_listener;
  int not_sender;

  // Send status.
  int send_val;
  int send_error;

  // Get the sender connection.
  sender_conn = (*conns)[conn_index];
  sender_fd   = sender_conn.fd;

  // Receive the data.
  data_len = recv(sender_fd, buffer, buf_len, 0);

  if (data_len <= 0) {

    recv_error    = (data_len < 0);
    disconnected  = (data_len == 0);

    // Process errors related to receiving data or if the connection terminated.
    if (recv_error)   perror("recv");
    if (disconnected) printf(
      "Connection terminated from client on socket %d.\n", sender_fd
    );

    close(sender_fd);
    
    remove_connection(conn_index, conns, conn_count);

  } else {

    // Send the data received to all of the client connections.
    for (conn_index = 0; conn_index < *conn_count; conn_index++) {
      
      client_fd = (*conns)[conn_index].fd;

      not_listener = (client_fd != listener_fd);
      not_sender   = (client_fd != sender_fd);

      if (not_listener && not_sender) {

        send_val    = send(client_fd, buffer, data_len, 0);
        send_error  = (send_val == -1);

        if (send_error) perror("send");

      }

    }

  }

}
