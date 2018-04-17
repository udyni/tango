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

#include "SerialSocket.h"


/*
 * Configure serial port settings
 */
void SerialSocket::configure_port(speed_t speed, char parity, uint8_t stopbits, uint8_t databits) {
	if(!isConnected())
		throw SocketException("configure_port(): not connected");

	// Structure to store the port settings
	struct termios port_settings;
	memset(&port_settings, 0, sizeof(struct termios));

	// Set baud rate
	cfsetispeed(&port_settings, speed);
	cfsetospeed(&port_settings, speed);

	port_settings.c_cflag |= CREAD|CLOCAL;

	// Set parity
	if(parity == 'N') {
		port_settings.c_cflag &= ~PARENB;
	} else {
		if(parity == 'O') {
			port_settings.c_cflag |= PARENB | PARODD;
		} else {
			port_settings.c_cflag |= PARENB;
			port_settings.c_cflag &= ~PARODD;
		}
	}

	// Set stopbits
	if(stopbits == 2)
		port_settings.c_cflag |= CSTOPB;
	else
		port_settings.c_cflag &= ~CSTOPB;

	// Set data size
	port_settings.c_cflag &= ~CSIZE;
	switch(databits) {
		case 5:
			port_settings.c_cflag |= CS5;
			break;
		case 6:
			port_settings.c_cflag |= CS6;
			break;
		case 7:
			port_settings.c_cflag |= CS7;
			break;
		case 8:
		default:
			port_settings.c_cflag |= CS8;
			break;
	}

	// Apply the settings to the port
	if(tcsetattr(fid, TCSANOW, &port_settings)) {
		throw SocketException("configure_port(): failed to set port parameters (Error: %s)", strerror(errno));
	}
}


//! Set serial port as raw (for binary communications)
void SerialSocket::set_raw() {
	if(!isConnected())
		throw SocketException("set_raw(): not connected.");

	// Structure to store the port settings
	struct termios port_settings;
	memset(&port_settings, 0, sizeof(struct termios));

	// Get port settings
	if(tcgetattr(fid, &port_settings)) {
		throw SocketException("set_raw(): failed to get port parameters (Error: %s)", strerror(errno));
	}

	// Raw input
	port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	// Enable parity check at input
	if(port_settings.c_cflag & PARENB)
		port_settings.c_iflag |= INPCK;

	// Software flow control is disabled
	port_settings.c_iflag &= ~(IXON | IXOFF | IXANY);

	// Raw output
	port_settings.c_oflag &=~ OPOST;

	// Apply the settings to the port
	if(tcsetattr(fid, TCSANOW, &port_settings)) {
		throw SocketException("set_raw(): failed to set port parameters (Error: %s)", strerror(errno))	;
	}
}


//! Open serial port
void SerialSocket::init_connection(const char* device) {
	// If we are already connected throw an exception
	if(isConnected())
		throw SocketException("init_connection(): already connected");

	// Open serial port
	fid = ::open(device, O_RDWR | O_NOCTTY | O_NDELAY);

	// Open failed
	if(fid == -1) {
		throw SocketException("init_connection(): error opening serial device (Error: %s)", strerror(errno));
	}
}


//! Close serial port
void SerialSocket::close_connection() {
	if(fid != -1)
		::close(fid);
	fid = -1;
}


//! Read block of data. Should be overloaded
int SerialSocket::read_block(char *buffer, size_t sz) {
	if(!isConnected())
		throw SocketException("read_block(): not connected");
	return ::read(fid, buffer, sz);
}


//! Read single char from buffer. Should be overloaded
int SerialSocket::read_char(char *buffer) {
	if(!isConnected())
		throw SocketException("read_char(): not connected");
	return ::read(fid, buffer, 1);
}


//! Send block of data. Should be overloaded
int SerialSocket::send_block(const char *buffer, size_t sz) {
	if(!isConnected())
		throw SocketException("send_block(): not connected");
	return ::write(fid, buffer, sz);
}


//! Handle read error. Should be overloaded
void SerialSocket::_handle_error(int err){
	switch(err) {
		case EBADF:
			fid = -1;
			break;

		case EINVAL:
		case EFAULT:
		case EPIPE:
		case EIO:
			::close(fid);
			fid = -1;

		default:
			break;
	}
}


//! Flush read buffer. Should be overloaded
void SerialSocket::flush_buffer() {
	if(!isConnected())
		throw SocketException("flush_buffer(): not connected");

	char buffer[32];
	while(1) {
		int ans = ::read(fid, buffer, 32);
		if(ans == -1) {
			if(errno == EAGAIN)
				break;
			if(errno == EINTR)
				continue;
			_handle_error(errno);
			throw SocketException("flush_buffer(): exception while flushing (Error: %s)", strerror(errno));
		}
		if(ans == 0)
			break;
	}
}


//! Poll file descriptor for data to read
int SerialSocket::poll(int timeout) {
	struct pollfd fds;
	fds.fd = fid;
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
