#ifndef REMOVE_CONNECTION_H_
#define REMOVE_CONNECTION_H_

typedef struct pollfd connection;

void remove_connection(
  int           conn_index,
  connection**  conns,
  int*          conn_count
);

#endif
