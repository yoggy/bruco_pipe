#include "udp.hpp"

#include <string>
#include <sstream>

#include "log.hpp"

int open_udp(const char *host, int port)
{
	int s;
	int rv;
	struct addrinfo hints;
	struct addrinfo *res = NULL;
	struct addrinfo *ai  = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	char port_str[16];
	snprintf(port_str, 15, "%d", port);

	rv = getaddrinfo(host, port_str, &hints, &res);
	if (rv) {
		log_e("getaddrinfo() failed...");
		return -1;
	}

	// connect
	s = -1;
	for (ai = res; ai != NULL; ai = ai->ai_next) {
		s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

		if (s < 0) continue;

		rv = connect(s, ai->ai_addr, ai->ai_addrlen);
		if (rv >= 0) {
			break;
		}
		s = -1;
	}

	if (s < 0) {
		log_e("connect() failed...host=%s, port=%d", host, port);
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);

	return s;
}

