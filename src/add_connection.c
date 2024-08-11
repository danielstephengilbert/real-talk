#include <poll.h>
#include <stdlib.h>

typedef struct pollfd connection;

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
