#ifndef ADD_CONNECTION_H_
#define ADD_CONNECTION_H_

typedef struct pollfd connection;

void add_connection(
  int           conn_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
);

#endif
