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

#include "gpio.h"

// Constructor
GPIO::GPIO(int gpio) :
	_gpio(gpio)
{
	char buffer[GPIO_MAX_PATH];
	int fd = -1;
	int ans = 0;

	// Check if the GPIO pin is already exported
	bool exported = false;

	ans = ::snprintf(buffer, GPIO_MAX_PATH, GPIO_SYSFS_BASE "/gpio%d", _gpio);
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to format path (Error: %s)", strerror(errno));
	}
	DIR* d = ::opendir(buffer);
	if(d) {
		exported = true;
		::closedir(d);
	} else {
		if(errno != ENOENT) {
			// Error
			throw GPIOException("Failed to open path (Error: %s)", strerror(errno));
		}
	}

	// If needed export the GPIO pins
	if(!exported) {
		fd = ::open(GPIO_SYSFS_BASE "/export", O_WRONLY);
		if(fd == -1) {
			// Error
			throw GPIOException("Failed to open export file (Error: %s)", strerror(errno));
		}

		ans = ::snprintf(buffer, GPIO_MAX_PATH, "%hu", _gpio);
		if(ans == -1) {
			// Error
			::close(fd);
			throw GPIOException("Failed to format number (Error: %s)", strerror(errno));
		}

		ans = ::write(fd, buffer, strlen(buffer));
		if(ans == -1) {
			// Error
			::close(fd);
			throw GPIOException("Failed to write GPIO number to export file (Error: %s)", strerror(errno));
		}
		::close(fd);
	}

	// Create paths to control files
	ans = snprintf(_value, GPIO_MAX_PATH, GPIO_SYSFS_BASE "/gpio%d/value", _gpio);
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to format value file path (Error: %s)", strerror(errno));
	}
	ans = snprintf(_direction, GPIO_MAX_PATH, GPIO_SYSFS_BASE "/gpio%d/direction", _gpio);
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to format direction file path (Error: %s)", strerror(errno));
	}
	ans = snprintf(_edge, GPIO_MAX_PATH, GPIO_SYSFS_BASE "/gpio%d/edge", _gpio);
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to format edge file path (Error: %s)", strerror(errno));
	}
}


// Destructor
GPIO::~GPIO() {
	// Unexport GPIO pin
	int fd = ::open(GPIO_SYSFS_BASE "/unexport", O_WRONLY);
	if(fd != -1) {
		char buffer[16];
		int ans = ::snprintf(buffer, 16, "%hu", _gpio);
		buffer[15] = '\0';
		if(ans != -1)
			::write(fd, buffer, strlen(buffer));
		::close(fd);
	}
}


// Read GPIO value
bool GPIO::read() {
	// Open GPIO value file
	int fd = ::open(_value, O_RDONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO value file for reading (Error: %s)", strerror(errno));
	}
	char value;
	int ans = ::read(fd, &value, 1);
	if(ans == -1) {
		// Error
		::close(fd);
		throw GPIOException("Failed to read GPIO value (Error: %s)", strerror(errno));
	}
	::close(fd);
	if(value == '0')
		return false;
	else
		return true;
}


// Write GPIO value
void GPIO::write(bool value) {
	// Open GPIO value file
	int fd = ::open(_value, O_WRONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO value file for writing (Error: %s)", strerror(errno));
	}
	const char *str;
	if(value)
		str = "1";
	else
		str = "0";
	int ans = ::write(fd, str, strlen(str));
	if(ans == -1) {
		// Error
		::close(fd);
		throw GPIOException("Failed to write GPIO value (Error: %s)", strerror(errno));
	}
}


int GPIO::getDirection() {
	// Open GPIO direction file
	int fd = ::open(_direction, O_RDONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO direction file for reading (Error: %s)", strerror(errno));
	}
	char value;
	ssize_t ans = ::read(fd, &value, 1);
	if(ans == -1) {
		// Error
		::close(fd);
		throw GPIOException("Failed to read GPIO direction (Error: %s)", strerror(errno));
	}
	::close(fd);
	if(value == 'o') {
		return GPIO_DIR_OUTPUT;
	} else if(value == 'i') {
		return GPIO_DIR_INPUT;
	} else {
		throw GPIOException("Got unexpected direction value '%c'", value);
	}
}


// Configure GPIO direction
void GPIO::setDirection(int direction) {
	const char *str;
	if(direction == GPIO_DIR_INPUT) {
		str = "in";
	} else if(direction == GPIO_DIR_OUTPUT) {
		str = "out";
	} else {
		// Bad value
		throw GPIOException("Got bad value for GPIO direction (%d)", direction);
	}

	// Open GPIO direction file
	int fd = ::open(_direction, O_WRONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO direction file for writing (Error: %s)", strerror(errno));
	}

	// Write direction
	int ans = ::write(fd, str, strlen(str));
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to write GPIO direction (Error: %s)", strerror(errno));
	}
}


int GPIO::getEdge() {
	// Open GPIO edge file
	int fd = ::open(_edge, O_RDONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO edge file for reading (Error: %s)", strerror(errno));
	}
	char value;
	ssize_t ans = ::read(fd, &value, 1);
	if(ans == -1) {
		// Error
		::close(fd);
		throw GPIOException("Failed to read GPIO edge (Error: %s)", strerror(errno));
	}
	::close(fd);
	if(value == 'n') {
		return GPIO_EDGE_NONE;
	} else if(value == 'r') {
		return GPIO_EDGE_RISING;
	} else if(value == 'f') {
		return GPIO_EDGE_FALLING;
	} else if(value == 'b') {
		return GPIO_EDGE_BOTH;
	} else {
		throw GPIOException("Got unexpected edge value '%c'", value);
	}
}


// Configure GPIO edge
void GPIO::setEdge(int edge) {
	const char *str;
	if(edge == GPIO_EDGE_NONE) {
		str = "none";
	} else if(edge == GPIO_EDGE_RISING) {
		str = "rising";
	} else if(edge == GPIO_EDGE_FALLING) {
		str = "falling";
	} else if(edge == GPIO_EDGE_BOTH) {
		str = "both";
	} else {
		// Bad value
		throw GPIOException("Got bad value for GPIO edge (%d)", edge);
	}

	// Open GPIO edge file
	int fd = ::open(_edge, O_WRONLY);
	if(fd == -1) {
		// Error
		throw GPIOException("Failed to open GPIO edge file for writing (Error: %s)", strerror(errno));
	}

	// Write edge
	int ans = ::write(fd, str, strlen(str));
	if(ans == -1) {
		// Error
		throw GPIOException("Failed to write GPIO edge (Error: %s)", strerror(errno));
	}
}


// Poll over multiple GPIO objects
int GPIO::poll(const std::vector<GPIO*>& pins, int timeout, std::vector<int>& triggered) {
	if(pins.size() == 0)
		return 0;

	std::vector<int> fd;
	struct pollfd* fds = NULL;

	triggered.clear();

	try {
		// Open files
		for(size_t i = 0; i < pins.size(); i++) {
			if(pins[i]) {
				int ans = ::open(pins[i]->_value, O_RDWR);
				if(ans == -1) {
					// Error
					throw GPIOException("Failed to open '%s' file for polling (Error: %s)", pins[i]->_value, strerror(errno));
				}
				fd.push_back(ans);
			} else {
				throw GPIOException("Passed a NULL pointer in GPIO pin list");
			}
		}

		// Format pollfd structs
		fds = new struct pollfd[fd.size()];
		memset(fds, 0, sizeof(struct pollfd) * fd.size());
		for(size_t i = 0; i < fd.size(); i++) {
			fds[i].fd = fd[i];
			fds[i].events = POLLPRI | POLLERR;
		}

		// Call poll
		int ans = ::poll(fds, fd.size(), timeout);
		if(ans == -1) {
			if(errno == EINTR) {
				// Poll returned because of a signal
				goto close;
			} else {
				// Poll failed
				throw GPIOException("Poll failed (Error: %s)", strerror(errno));
			}

		} else if(ans == 0) {
			// Poll returned because of timeout
			goto close;

		} else if(ans > 0) {
#ifdef _DEBUG
			printf("Poll result: %d\n", ans);
#endif
			// Poll triggered by GPIO interrupt
			for(size_t i = 0; i < fd.size(); i++) {
#ifdef _DEBUG
				printf("FD %d output flags: ", i);
				if(fds[i].revents & POLLIN)
					printf("POLLIN ");
				if(fds[i].revents & POLLPRI)
					printf("POLLPRI ");
				if(fds[i].revents & POLLOUT)
					printf("POLLOUT ");
				if(fds[i].revents & POLLRDHUP)
					printf("POLLRDHUP ");
				if(fds[i].revents & POLLERR)
					printf("POLLERR ");
				if(fds[i].revents & POLLHUP)
					printf("POLLHUP ");
				if(fds[i].revents & POLLNVAL)
					printf("POLLNVAL ");
				printf("\n");
#endif
				if(fds[i].revents & (POLLPRI | POLLERR)) {
					triggered.push_back(1);
				} else {
					triggered.push_back(0);
				}
			}
			goto close;

		} else {
			// This case should never happen
			throw GPIOException("The program should never reach this point! Poll returned unexpected value %d", ans);
		}

		close:
		for(size_t i = 0; i < fd.size(); i++)
 			::close(fd[i]);
		delete[] fds;
		return ans;

	} catch(GPIOException &e) {
		if(fds)
			delete[] fds;
		for(size_t i = 0; i < fd.size(); i++)
			::close(fd[i]);
		throw e;
	}
}
