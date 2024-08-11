#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct sockaddr address;

/**
 * Get the correct IPv4/6 struct for an address.
 */
const void* get_ipv46_addr(
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
