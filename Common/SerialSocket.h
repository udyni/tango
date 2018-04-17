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

#ifndef SERIAL_SOCKET_H
#define SERIAL_SOCKET_H

#include "CommSocket.h"
#include <termios.h>


/* @class SerialSocket
 * Implement communication through a serial port
 */
class SerialSocket : public CommSocket {
public:
	//! Constructors
	SerialSocket() : fid(-1) {};
	SerialSocket(const char* device) : fid(-1) { init_connection(device); };
	SerialSocket(const std::string &device) : fid(-1) { init_connection(device.c_str()); };

private:
	//! Private copy constructor and private assignment operator to prevent class copying
	SerialSocket(const SerialSocket& obj) : CommSocket() {};
	SerialSocket& operator=(const SerialSocket& obj) {};

public:
	//! Destructor
	~SerialSocket() { close_connection(); };

	// Configure serial port
	void configure_port(speed_t speed, char parity='N', uint8_t stopbits=1, uint8_t databits=8);

	// Configure serial port as raw
	void set_raw();

private:
	//! Check if a serial port is open and connected
	bool check_connection()const { if(fid != -1) return true; else return false; }

	//! Open serial port
	void init_connection(const char* device);

	//! Close serial port
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

	//! Poll file descriptor for data to read
	int poll(int timeout);

private:
	// File descriptor
	int fid;
};

#endif