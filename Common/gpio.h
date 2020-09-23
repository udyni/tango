/**
 * (c) 2015 - Michele Devetta (michele.devetta@mail.polimi.it)
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

#ifndef GPIO_COMMON_H
#define GPIO_COMMON_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <dirent.h>
#include <vector>
#include <exception>

#define GPIO_SYSFS_BASE      "/sys/class/gpio"
#define GPIO_MAX_PATH        256
#define GPIO_DIR_INPUT       0
#define GPIO_DIR_OUTPUT      1
#define GPIO_DIR_OUTPUT_HIGH 2
#define GPIO_DIR_OUTPUT_LOW  3
#define GPIO_EDGE_NONE       0
#define GPIO_EDGE_RISING     1
#define GPIO_EDGE_FALLING    2
#define GPIO_EDGE_BOTH       3

#define GPIO_ERROR_MSG_LEN 256


// @class GPIOException
class GPIOException : public std::exception {
public:
	//! Constructor
	/*! Constructor of the exception. Can format an error message.
	 *  @param f the format string (as in printf)
	 *  @param ... variable parameters, corresponding to the format string.
	 */
	GPIOException(const char* f, ...) {
		va_list ap;
		va_start(ap, f);
		vsnprintf(_msg, GPIO_ERROR_MSG_LEN, f, ap);
		va_end(ap);
	}
	GPIOException(const char* f, va_list ap) {
		vsnprintf(_msg, GPIO_ERROR_MSG_LEN, f, ap);
	}

	//! what(), return the error string
	virtual const char* what() const throw() {
		return _msg;
	}

private:
	char _msg[GPIO_ERROR_MSG_LEN];
};


// @class GPIO
class GPIO {
public:
	// Constructor
	GPIO(int gpio);

	// Destructor
	~GPIO();

public:
	// Read GPIO value
	bool read();

	// Write GPIO value
	void write(bool value);

	// Read GPIO direction
	int getDirection();

	// Configure GPIO direction
	void setDirection(int direction);

	// Get GPIO edge
	int getEdge();

	// Configure GPIO edge
	void setEdge(int edge);

	// Poll over multiple GPIO objects
	static int poll(const std::vector<GPIO*>& pins, int timeout, std::vector<int>& triggered);

	// Get GPIO pin
	int pin() { return _gpio; }

private:
	int _gpio;                      // GPIO number
	char _value[GPIO_MAX_PATH];     // Value file
	char _direction[GPIO_MAX_PATH]; // Direction file
	char _edge[GPIO_MAX_PATH];      // Edge file
};

#endif
