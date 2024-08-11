#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>

#include "../include/add_connection.h"
#include "../include/get_ipv46_addr.h"

typedef struct pollfd    connection;
typedef struct sockaddr  address;

/**
 * Process new connection requests via the listener.
 */
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
    addr_struct = get_ipv46_addr((address *) &client_addr);

    client_addr_str = inet_ntop(
      addr_family, addr_struct, addr_buffer, max_addr_len
    );
    
    printf(
      "Connection established from %s on socket %d.\n", client_addr_str, conn
    );
    
  }

}
