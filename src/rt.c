#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct addrinfo   addr_info;
typedef struct pollfd     conn;
typedef struct sockaddr   addr;

int get_listener(
  void
);

const void* get_in_addr(
  addr* addr
);

void add_connection(
  conn** conns, int conn_fd, int* conn_count, int* max_conns
);

void remove_conn(
  conn** conns, int conn_index, int* conn_count
);

void process_listener(
  int listener_fd, conn** conns, int* conn_count, int* max_conns
);

void process_client(
  int conn_index, int listener_fd, conn** conns, int* conn_count
);

int main(void) {

  int     max_conns;
  conn*   conns;
  int     conns_size;
  int     listener_fd;
  int     listener_error;
  int     conn_count;
  int     poll_val;
  int     poll_error;
  int     conn_index;
  conn*   conn;
  int     data_ready;
  int     listener_data;
  int     client_data;

  max_conns   = 5;
  conns_size  = sizeof(conn) * max_conns;
  conns       = malloc(conns_size);

  listener_fd     = get_listener();
  listener_error  = (listener_fd == -1);

  if (listener_error) {
    fprintf(stderr, "Error: Could not start listener.\n");
    exit(1);
  }

  conns[0].fd       = listener_fd;
  conns[0].events   = POLLIN;

  conn_count        = 1;

  while (1) {

    poll_val    = poll(conns, conn_count, -1);
    poll_error  = (poll_val == -1);

    if (poll_error) {
      perror("poll");
      exit(1);
    }

    for (conn_index = 0; conn_index < conn_count; conn_index++) {

      conn          = &conns[conn_index];
      
      data_ready    = (conn->revents & POLLIN);
      listener_data = (conn->fd == listener_fd);
      client_data   = !listener_data;

      if (data_ready && listener_data) {

        process_listener(listener_fd, &conns, &conn_count, &max_conns);

      } else if (data_ready && client_data) {

        process_client(conn_index, listener_fd, &conns, &conn_count);

      }

    }

  }

  return 0;

}

int get_listener(void) {

  addr_info     addr_spec;
  int           getaddrinfo_val;
  char*         port = "55555";
  addr_info*    first_addr;
  int           getaddrinfo_error;
  addr_info*    addr_iter;
  int           listener_fd;
  int           listener_invalid;
  int           protocol_level = 1;
  int           bind_val;
  int           bind_error;
  int           bind_failed;
  int           listen_val;
  int           listen_error;

  memset(&addr_spec, 0, sizeof(addr_spec));

  addr_spec.ai_family   = AF_UNSPEC;
  addr_spec.ai_socktype = SOCK_STREAM;
  addr_spec.ai_flags    = AI_PASSIVE;

  getaddrinfo_val   = getaddrinfo(NULL, port, &addr_spec, &first_addr);
  getaddrinfo_error = (getaddrinfo_val != 0);

  if (getaddrinfo_error) {
    fprintf(stderr, "Error: %s\n", gai_strerror(getaddrinfo_val));
    exit(1);
  }

  for (
    addr_iter = first_addr; addr_iter != NULL; addr_iter = addr_iter->ai_next
  ) {
    
    listener_fd = socket(
      addr_iter->ai_family, addr_iter->ai_socktype, addr_iter->ai_protocol
    );
    listener_invalid = (listener_fd < 0);

    if (listener_invalid) continue;

    setsockopt(
      listener_fd, SOL_SOCKET, SO_REUSEADDR, &protocol_level, sizeof(int)
    );

    bind_val    = bind(listener_fd, addr_iter->ai_addr, addr_iter->ai_addrlen);
    bind_error  = (bind_val < 0);

    if (bind_error) {
      close(listener_fd);
      continue;
    }

    break;

  }

  freeaddrinfo(first_addr);

  bind_failed = (addr_iter == NULL);
  if (bind_failed) return -1;

  listen_val    = listen(listener_fd, 10);
  listen_error  = (listen_val == -1);
  if (listen_error) return -1;

  return listener_fd;

}

const void* get_in_addr(
  addr* addr
) {

  typedef struct sockaddr_in  ipv4_struct;
  typedef struct sockaddr_in6 ipv6_struct;

  int                 is_ipv4;
  ipv4_struct*        ipv4;
  struct in_addr*     ipv4_addr;

  int                 is_ipv6;
  ipv6_struct*        ipv6;
  struct in6_addr*    ipv6_addr;

  is_ipv4 = (addr->sa_family == AF_INET);
  is_ipv6 = (addr->sa_family == AF_INET6);

  if (is_ipv4) {

    ipv4        = (ipv4_struct*) addr;
    ipv4_addr   = &(ipv4->sin_addr);

    return ipv4_addr;

  } else if (is_ipv6) {

    ipv6        = (ipv6_struct*) addr;
    ipv6_addr   = &(ipv6->sin6_addr);

    return ipv6_addr;

  } else {

    fprintf(stderr, "Error: Could not obtain address.\n");
    exit(1);

  }

}

void add_connection(
  conn** conns, int conn_fd, int* conn_count, int* max_conns
) {

  int need_space;
  int space_multiplier = 2;
  int conns_size;

  need_space = (*conn_count == *max_conns);

  if (need_space) {

    *max_conns      *= space_multiplier;
    conns_size       = sizeof(conn) * (*max_conns);

    *conns = realloc(*conns, conns_size);

  }

  (*conns)[*conn_count].fd      = conn_fd;
  (*conns)[*conn_count].events  = POLLIN;

  (*conn_count)++;

}

void remove_conn(
  conn** conns, int conn_index, int* conn_count
) {

  int last_conn_index   = *conn_count - 1;

  (*conns)[conn_index]  = (*conns)[last_conn_index];

  (*conn_count)--;

}

void process_listener(
  int listener_fd, conn** conns, int* conn_count, int* max_conns
) {

  int                       conn;
  struct sockaddr_storage   client_addr;
  addr*                     client;
  socklen_t                 addr_len = sizeof(client_addr);
  int                       accept_error;
  int                       addr_family;
  const void*               addr_struct;
  const char*               client_addr_str;
  int                       max_addr_len = INET6_ADDRSTRLEN;
  char                      addr_buffer[max_addr_len];

  client  = (addr *) &client_addr;
  conn    = accept(listener_fd, client, &addr_len);

  accept_error = (conn == -1);

  if (accept_error) {

    perror("accept");

  } else {
    
    add_connection(conns, conn, conn_count, max_conns);
    
    addr_family     = client_addr.ss_family;
    addr_struct     = get_in_addr((addr *) &client_addr);

    client_addr_str = inet_ntop(
      addr_family, addr_struct, addr_buffer, max_addr_len
    );
    
    printf(
      "Connection established from %s on socket %d.\n", client_addr_str, conn
    );
    
  }

}

void process_client(
  int conn_index, int listener_fd, conn** conns, int* conn_count
) {

  conn      sender_conn;
  int       sender_fd;
  int       buf_len = 256;
  char      buffer[buf_len];
  int       data_len;
  int       recv_error;
  int       disconnected;
  int       client_fd;
  int       not_listener;
  int       not_sender;
  int       send_val;
  int       send_error;

  sender_conn = (*conns)[conn_index];
  sender_fd   = sender_conn.fd;

  data_len = recv(sender_fd, buffer, buf_len, 0);

  if (data_len <= 0) {

    recv_error    = (data_len < 0);
    disconnected  = (data_len == 0);

    if (recv_error)   perror("recv");
    if (disconnected) printf(
      "Connection terminated from client on socket %d.\n", sender_fd
    );

    close(sender_fd);
    
    remove_conn(conns, conn_index, conn_count);

  } else {

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
