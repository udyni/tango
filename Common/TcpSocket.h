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

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "CommSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


/* @class TcpSocket
 * Implement communication through a TCP connection
 */
class TcpSocket : public CommSocket {
public:
	//! Constructors
	TcpSocket() : sock(-1) {};
	TcpSocket(const char* device) : sock(-1) { init_connection(device); };
	TcpSocket(const std::string &device) : sock(-1) { init_connection(device.c_str()); };

private:
	//! Private copy constructor and private assignment operator to prevent class copying
	TcpSocket(const TcpSocket& obj);
	TcpSocket& operator=(const TcpSocket& obj);

public:
	//! Destructor
	~TcpSocket() { close_connection(); };

private:
	//! Check if a TCP connection is open
	bool check_connection()const { if(sock != -1) return true; else return false; }

	//! Open TCP connection
	void init_connection(const char* device);

	//! Close TCP connection
	void close_connection();

	//! Read block of data
	int read_block(char *buffer, size_t sz);

	//! Read single char from buffer
	int read_char(char *buffer);

	//! Send block of data
	int send_block(const char *buffer, size_t sz);

	//! Handle read error
	void _handle_error(int err);

	//! Flush read buffer
	void flush_buffer();

	//! Poll socket for data to read
	int poll(int timeout);

private:
	// Socket descriptor
	int sock;

	// Address
	struct sockaddr_in addr;
};

#endif
