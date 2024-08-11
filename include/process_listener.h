#ifndef PROCESS_LISTENER_H_
#define PROCESS_LISTENER_H_

typedef struct pollfd connection;

void process_listener(
  int           listener_fd,
  connection**  conns,
  int*          conn_count,
  int*          max_conns
);

#endif
