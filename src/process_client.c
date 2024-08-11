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
