/**
 * (c) 2014 - Michele Devetta (michele.devetta@mail.polimi.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "TcpSocket.h"


//! Open TCP connection
void TcpSocket::init_connection(const char* device) {
	// If we are already connected throw an exception
	if(isConnected())
		throw SocketException("init_connection(): already connected.");


	// Split hostname and port
	// 1) Copy the string
	char *host = new char[strlen(device)+1];
	strncpy(host, device, strlen(device)+1);
	// 2) Find separator
	char *port_str = strchr(host, ':');
	if(port_str == NULL)
		throw SocketException("init_connection(): malformed device address (Device was: %s)", device);
	// 3) Split string
	port_str[0] = 0x00;
	port_str = port_str + 1;
	// 4) Convert port to uint16_t
	uint16_t port = atoi(port_str);

	// Check if hostname is a name or IP...
	struct addrinfo hints;
	struct addrinfo *result;
	// Hints...
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	// Get address info
	int s = getaddrinfo(host, NULL, &hints, &result);
	if (s != 0) {
		throw SocketException("init_connection(): cannot resolve address (Error: %s)", gai_strerror(s));
	}
	// getaddrinfo() returns a list of addresses. We take the first one.
	memcpy(&addr, result->ai_addr, sizeof(struct sockaddr));
	freeaddrinfo(result);
	delete[] host;

	// Set port
	addr.sin_port = htons(port);

	// Create socket
	sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		throw SocketException("init_connection(): error creating socket (Error: %s)", strerror(errno));
	}

	// Set socket as non blocking
	// Get socket flags
	int opts = ::fcntl(sock, F_GETFL);
	if(opts < 0) {
		// Error
		throw SocketException("init_connection(): error getting socket flags (Error: %s)", strerror(errno));
	}
	opts = (opts | O_NONBLOCK);
	// Set socket flags
	::fcntl(sock, F_SETFL, opts);

	// Connect
	if(::connect(sock, (struct sockaddr*)&addr, sizeof(addr))) {
		// If error is not EINPROGRESS
		if(errno != EINPROGRESS) {
			// Connect error
			::close(sock);
			sock = -1;
			throw SocketException("init_connection(): error connecting (Error: %s)", strerror(errno));

		} else {
			// Timeout for connect
			struct timeval ts, te;
			memset(&ts, 0, sizeof(ts));
			memset(&te, 0, sizeof(te));
			gettimeofday(&ts,  NULL);

			// Poll waiting for writing
			struct pollfd fds;

			while(true) {
				// Check timeout
				gettimeofday(&te, NULL);
				if(ELAPSED_TIME_MS(ts, te) > SOCK_DEF_TIMEOUT) {
					// Timed out. Connection failed
					::close(sock);
					sock = -1;
					throw SocketException("init_connection(): connection timed out.");
				}

				memset(&fds, 0, sizeof(fds));
				fds.fd = sock;
				fds.events = POLLIN|POLLOUT;
				int ans = ::poll(&fds, 1, 100);
				if(ans == -1) {
					if(errno == EINTR)
						continue;
					// Poll error
					::close(sock);
					sock = -1;
					throw SocketException("init_connection(): error during poll (Error: %s)", strerror(errno));

				} else if(ans == 0) {
					// Timed out
					msleep(5);
					continue;
				} else {
					// Socket ready. Check connection status
					int sock_err = 0;
					socklen_t len = sizeof(int);
					if(getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_err, &len)) {
						// Error getting SO_ERROR
						::close(sock);
						sock = -1;
						throw SocketException("init_connection(): error during getsockopt (Error: %s)", strerror(errno));
					}
					if(sock_err) {
						// Connect error
						::close(sock);
						sock = -1;
						throw SocketException("init_connection(): connection failed (Error: %s)", strerror(sock_err));
					} else {
						// Connection succeeded!
						refcount = new int();
						*refcount = 1;
						return;
					}
				}
			}
		}

	} else {
		// Connection succeeded at first try!
		refcount = new int();
		*refcount = 1;
	}
}


//! Close TCP connection
void TcpSocket::close_connection() {
	if(sock != -1) {
		if(refcount) {
			if(*refcount <= 1) {
				::close(sock);
				delete refcount;
				refcount = NULL;
				sock = -1;
			} else {
				// Decrement reference count
				(*refcount)--;
			}
		} else {
			::close(sock);
			sock = -1;
		}
	}
}


//! Read block of data
int TcpSocket::read_block(char *buffer, size_t sz) {
	if(!isConnected())
		throw SocketException("read_block(): not connected.");
	return ::recv(sock, buffer, sz, 0);
}


//! Read single char from buffer
int TcpSocket::read_char(char *buffer) {
	if(!isConnected())
		throw SocketException("read_char(): not connected.");
	return ::recv(sock, buffer, 1, 0);
}


//! Send block of data
int TcpSocket::send_block(const char *buffer, size_t sz) {
	if(!isConnected())
		throw SocketException("send_block(): not connected.");
	return ::send(sock, buffer, sz, 0);
}


//! Handle read error
void TcpSocket::_handle_error(int err) {
	switch(err) {
		case EBADF:
		case ENOTSOCK:
			sock = -1;
			break;

		case ECONNRESET:
		case ENOTCONN:
		case EPIPE:
			::close(sock);
			sock = -1;

		default:
			break;
	}

	// Restore errno to previous value
	errno = err;
}


//! Flush read buffer
void TcpSocket::flush_buffer() {
	if(!isConnected())
		throw SocketException("flush_buffer(): not connected.");

	char buffer[32];
	while(1) {
		int ans = ::recv(sock, buffer, 32, 0);
		if(ans == -1) {
			if(errno == EAGAIN)
				break;
			if(errno == EINTR)
				continue;
			_handle_error(errno);
			throw SocketException("flush_buffer(): exception while flushing (Error: %s).", strerror(errno));
		}
		if(ans == 0)
			break;
	}
}


//! Poll socket for data to read
int TcpSocket::poll(int timeout) {
	struct pollfd fds;
	fds.fd = sock;
	fds.events = POLLIN | POLLPRI;

	int ans = ::poll(&fds, 1, timeout);
	if(ans < 0) {
		// Error
		if(errno == EINTR)
			return 0;
		else
			throw SocketException("poll(): failed to poll for input data (Error: %s)", strerror(errno));

	} else if(ans == 0) {
		// No data to read
		return 0;

	} else {
		// Socket ready for a read
		if(fds.revents & POLLERR) {
			// Socket error
			this->close_connection();
			throw SocketException("poll(): poll failed because of connection error");

		} else if(fds.revents & POLLHUP) {
			// Socket closed
			this->close_connection();
			throw SocketException("poll(): poll failed because connection was closed");

		} else {
			return 1;
		}
	}
}
