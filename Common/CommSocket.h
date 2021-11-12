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

#ifndef COMM_SOCKET_H
#define COMM_SOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <string>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <poll.h>

#define SOCK_ERR_MSG_LEN   256
#define SOCK_DEF_RETRIES     5
#define SOCK_DEF_TIMEOUT  2000
#define SOCK_DEF_BLOCK     256

#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif

/* @class SocketException
 * Default exception for communication socket classes
 * Extends std::exception to be able to format messages
 */
class SocketException : public std::exception {
public:
	//! Constructor
	/*! Constructor of the exception. Can format an error message.
	 *  @param f the format string (as in printf)
	 *  @param ... variable parameters, corresponding to the format string.
	 */
	SocketException(const char* f, ...) {
		va_list ap;
		va_start(ap, f);
		vsnprintf(_msg, SOCK_ERR_MSG_LEN, f, ap);
		va_end(ap);
	}
	SocketException(const char* f, va_list ap) {
		vsnprintf(_msg, SOCK_ERR_MSG_LEN, f, ap);
	}

	//! what(), return the error string
	virtual const char* what() const throw() {
		return _msg;
	}

private:
	char _msg[SOCK_ERR_MSG_LEN];
};


/* @class CommSocket
 * Base class for socket communication
 */
class CommSocket {
public:
	//! Constructor
	CommSocket() : refcount(NULL) {}

private:
	//! Private copy constructor and private assignment operator to prevent base class copying
	CommSocket(const CommSocket& obj);
	CommSocket& operator=(const CommSocket& obj);

public:
	//! Destructor
	virtual ~CommSocket() {}

	// Connect interface
	void connect(const std::string &device) { this->connect(device.c_str()); }
	void connect(const char* device) { init_connection(device); };

	// Disconnect interface
	void disconnect() { close_connection(); };

	// Receive interface
	int recv(char* buffer, size_t sz, int timeout=SOCK_DEF_TIMEOUT);
	int recv_msg(std::string& buffer, const char* terminator, int timeout=SOCK_DEF_TIMEOUT);

	// Send interface
	int send(const char* msg, size_t sz);
	int send(const std::string msg) { return send(msg.c_str(), msg.size()); };

	// Flush read buffer
	void flush() { flush_buffer(); };

	// Options
	bool isConnected()const { return check_connection(); };
	int getRefCount()const { if(refcount) return *refcount; else return 0; };

	//! Virtual clone method
	//virtual CommSocket* clone() const = 0;
	
	// Millisecond sleep
	void msleep(uint32_t delay);

private:
	//! Check connection state. Should be overloaded.
	virtual bool check_connection()const = 0;

	//! Connection stub. Should be overloaded
	virtual void init_connection(const char* device) = 0;

	//! Disconnection stub. Should be overloaded
	virtual void close_connection() = 0;

	//! Read block of data. Should be overloaded
	virtual int read_block(char *buffer, size_t sz) = 0;

	//! Read single char from buffer. Should be overloaded
	virtual int read_char(char *buffer) = 0;

	//! Send block of data. Should be overloaded
	virtual int send_block(const char *buffer, size_t sz) = 0;

	//! Handle read error. Should be overloaded
	virtual void _handle_error(int err) = 0;

	//! Flush read buffer. Should be overloaded
	virtual void flush_buffer() = 0;

	//! Poll socket for data to read
	virtual int poll(int timeout) = 0;

protected:
	//! Reference count
	int *refcount;
};

#endif
