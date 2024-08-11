#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/get_listener.h"

typedef struct addrinfo  addr_info;
typedef struct pollfd    connection;
typedef struct sockaddr  address;

const void* get_in_addr(
  address* addr
);

void add_connection(
  int           conn_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
);

void remove_conn(
  int           conn_index,
  connection**  conns,
  int*          conn_count
);

void process_listener(
  int           listener_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
);

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
 * Get the correct IPv4/6 struct for an address.
 */
const void* get_in_addr(
  address* addr
) {

  // Declare local typedefs.
  typedef struct sockaddr_in  ipv4_struct;
  typedef struct sockaddr_in6 ipv6_struct;

  // IPv4 vars.
  int                 is_ipv4;
  ipv4_struct*        ipv4;
  struct in_addr*     ipv4_addr;

  // IPv6 vars.
  int                 is_ipv6;
  ipv6_struct*        ipv6;
  struct in6_addr*    ipv6_addr;

  // IP address type check boolean vars.
  is_ipv4 = (addr->sa_family == AF_INET);
  is_ipv6 = (addr->sa_family == AF_INET6);

  // If IPv4 address, get the correct struct format.
  if (is_ipv4) {

    ipv4        = (ipv4_struct*) addr;
    ipv4_addr   = &(ipv4->sin_addr);

    return ipv4_addr;

  // If IPv6 address, get the correct struct format.
  } else if (is_ipv6) {

    ipv6        = (ipv6_struct*) addr;
    ipv6_addr   = &(ipv6->sin6_addr);

    return ipv6_addr;

  // If neither IPv4 or IPv6, throw error.
  } else {

    fprintf(stderr, "Error: Could not obtain address.\n");
    exit(1);

  }

}

/**
 * Add a connection to the server.
 */
void add_connection(
  int           conn_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
) {

  // Vars related to space allocation for the connections.
  int need_space;
  int space_multiplier = 2;
  int conns_size;

  // If space is maxed out, reallocate additional space.
  need_space = (*conn_count == *max_conns);
  if (need_space) {

    *max_conns      *= space_multiplier;
    conns_size       = sizeof(connection) * (*max_conns);

    *conns = realloc(*conns, conns_size);

  }

  // Add the new connection.
  (*conns)[*conn_count].fd      = conn_fd;
  (*conns)[*conn_count].events  = POLLIN;
  (*conn_count)++;

}

/**
 * Remove a connection.
 */
void remove_conn(
  int           conn_index,
  connection**  conns,
  int*          conn_count
) {

  // Remove the connection at index,
  // and replace it with the last connection
  // to make space at the end of the array for new connections.
  int last_conn_index   = *conn_count - 1;
  (*conns)[conn_index]  = (*conns)[last_conn_index];
  (*conn_count)--;

}

void process_listener(
  int           listener_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
) {

  // Connection var.
  int                       conn;

  // Client info vars.
  address*                 client;
  struct sockaddr_storage  client_addr;
  const char*              client_addr_str;

  // Address info vars.
  socklen_t    addr_len = sizeof(client_addr);
  int          addr_family;
  const void*  addr_struct;
  int          max_addr_len = INET6_ADDRSTRLEN;
  char         addr_buffer[max_addr_len];

  // Accept error var.
  int accept_error;

  // Get client address and try to accept connection.
  client  = (address *) &client_addr;
  conn    = accept(listener_fd, client, &addr_len);

  // Throw error if cannot accept connection.
  accept_error = (conn == -1);
  if (accept_error) {
    perror("accept");
  // Add the connection if successfully connected.
  } else {
    
    add_connection(conn, conns, conn_count, max_conns);
    
    addr_family = client_addr.ss_family;
    addr_struct = get_in_addr((address *) &client_addr);

    client_addr_str = inet_ntop(
      addr_family, addr_struct, addr_buffer, max_addr_len
    );
    
    printf(
      "Connection established from %s on socket %d.\n", client_addr_str, conn
    );
    
  }

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
    
    remove_conn(conn_index, conns, conn_count);

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
