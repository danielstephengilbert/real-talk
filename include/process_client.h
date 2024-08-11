#ifndef PROCESS_CLIENT_H_
#define PROCESS_CLIENT_H_

typedef struct pollfd connection;

void process_client(
  int           conn_index,
  connection**  conns,
  int*          conn_count,
  int           listener_fd
);

#endif
