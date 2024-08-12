#ifndef RUN_SERVER_H_
#define RUN_SERVER_H_

typedef struct pollfd connection;

void run_server(
  connection**  conns,
  int*          conn_count,
  int*          max_conns,
  int           listener_fd
);

#endif
