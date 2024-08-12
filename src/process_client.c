#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/remove_connection.h"

typedef struct pollfd connection;

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

  // If data length indicates an error, handle the error then return;
  if (data_len <= 0) {

    // Check specific error conditions.
    recv_error    = (data_len < 0);
    disconnected  = (data_len == 0);

    // Throw error if could not receive data.
    if (recv_error) perror("recv");

    // Throw error if connection terminated.
    if (disconnected) printf(
      "Connection terminated from client on socket %d.\n", sender_fd
    );

    // Close and remove the connection if any errors, then return.
    close(sender_fd);
    remove_connection(conn_index, conns, conn_count);
    return;

  }

  // Send the data received to all of the client connections.
  for (conn_index = 0; conn_index < *conn_count; conn_index++) {
    
    // Get the client connection.
    client_fd = (*conns)[conn_index].fd;

    // Check if current connection listener or sender.
    not_listener = (client_fd != listener_fd);
    not_sender   = (client_fd != sender_fd);

    // Do not send data to listener or sender.
    if (not_listener && not_sender) {

      // Send the data, and capture any error.
      send_val    = send(client_fd, buffer, data_len, 0);
      send_error  = (send_val == -1);

      // Print error if cannot send.
      if (send_error) perror("send");

    }

  }

}
