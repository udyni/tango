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

#include "CommSocket.h"


int CommSocket::recv(char* buffer, size_t sz, int timeout) {
	if(!isConnected())
		throw SocketException("recv(): not connected.");

	// Init timeout structs
	struct timeval ts, te;
	memset(&ts, 0, sizeof(ts));
	memset(&te, 0, sizeof(te));
	gettimeofday(&ts,  NULL);

	// Allocate read buffer
	int dataleft = sz;
	int dataread = 0;

	// Try to get the specified amount of data within timeout ms
	while(1) {
		// Get current time
		gettimeofday(&te, NULL);
		int elapsed = ELAPSED_TIME_MS(ts, te);

		// Check timeout
		if(elapsed >= timeout) {
			// If we have a partial result we return it
			if(dataread > 0) {
				return dataread;
			// Otherwise we return an error
			} else {
				errno = EAGAIN;
				return -1;
			}
		}

		// Poll socket
		int ans = this->poll(timeout - elapsed);

		// Read data
		if(ans > 0) {
			ans = this->read_block(buffer + dataread, dataleft);

			// Read error
			if(ans == -1) {
				// In case of blocking operation or interrupt, try again
				if(errno == EAGAIN || errno == EINTR) {
					continue;
				}

				// For other errors throw exception
				_handle_error(errno);
				throw SocketException("recv(): error receiving data (Error: %s)", strerror(errno));

			// Read done
			} else {
				dataleft -= ans;
				dataread += ans;
			}

			if(dataleft <= 0)
				break;
		}
	}

	return dataread;
}


/**
 * Receive a message terminated by 'terminator'
 */
int CommSocket::recv_msg(std::string& buffer, const char* terminator, int timeout) {
	if(!isConnected())
		throw SocketException("recv_msg(): not connected.");

	// Init timeout structs
	struct timeval ts, te;
	memset(&ts, 0, sizeof(ts));
	memset(&te, 0, sizeof(te));
	gettimeofday(&ts,  NULL);

	// Clear buffer
	buffer.clear();
	buffer.reserve(SOCK_DEF_BLOCK);

	while(true) {
		// Get current time
		gettimeofday(&te, NULL);
		int elapsed = ELAPSED_TIME_MS(ts, te);

		// Check timeout
		if(elapsed >= timeout) {
			// NOTE: partial messages are ignored as in datagram mode they make no sense...
			errno = EAGAIN;
			return -1;
		}

		// Poll socket
		int ans = this->poll(timeout - elapsed);

		// Receive one byte
		char bt;
		ans = read_char(&bt);
		if(ans == -1) {
			// Try again
			if(errno == EAGAIN || errno == EINTR) {
				continue;
			}
			// For other errors throw exception
			_handle_error(errno);
			throw SocketException("recv(): error sending message (Error: %s)", strerror(errno));

		} else if(ans == 0) {
			continue;

		} else {
			// Pre-allocate
			if(buffer.size()+1 > buffer.capacity())
				buffer.reserve(buffer.size()+SOCK_DEF_BLOCK);
			// Add byte to buffer
			buffer += bt;
			// Check if we got the termination
			size_t pos = buffer.find(terminator);
			if(pos == std::string::npos)
				continue; // Not found...
			// Found! Erase terminator and return
			buffer.erase(pos, std::string::npos);
			return buffer.size();
		}
	}
}


/**
 * Send a message to the socket
 */
int CommSocket::send(const char* msg, size_t sz) {
	if(!isConnected())
		throw SocketException("send(): not connected.");

	// Try to send the message SOCK_DEF_RETRIES times
	int count = 0;
	int ans = -1;
	while(ans == -1) {
		// Send message
		ans = send_block(msg, sz);
		if(ans == -1) {
			// Operation interrupted or operation would block
			if(errno == EAGAIN || errno == EINTR) {
				// Retry unless reached the max number of attempts
				if(count >= SOCK_DEF_RETRIES) {
					errno = EAGAIN;
					return -1;
				} else {
					count++;
					continue;
				}
			}

			// For other errors throw exception
			_handle_error(errno);
			throw SocketException("send(): error sending message (Error: %s)", strerror(errno));
		}
	}
	return ans;
}


/**
 * Sleep for the given number of msec
 */
void CommSocket::msleep(uint32_t msec) {
	if(msec <= 0)
		return;
	struct timespec st;
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	// Setup delay
	st.tv_sec = msec / 1000;
	st.tv_nsec = (msec % 1000) * 1000000;

	while(1) {
		int ans = nanosleep(&st, NULL);
		if(ans == -1 && errno == EINTR) {
			// Nanosleep was interrupted by a signal
			gettimeofday(&stop, NULL);
			uint32_t rem = msec - ELAPSED_TIME_MS(start, stop);
			if(rem > 0) {
				st.tv_sec = rem / 1000;
				st.tv_nsec = (rem % 1000) * 1000000;
			} else {
				break;
			}
		} else {
			break;
		}
	}
}
