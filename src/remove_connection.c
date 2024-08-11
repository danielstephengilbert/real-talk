#include <poll.h>

typedef struct pollfd connection;

/**
 * Remove a connection.
 */
void remove_connection(
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
