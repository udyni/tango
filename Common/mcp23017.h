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

#ifndef MCP23017_H
#define MCP23017_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


// MCP23017 registers
#define MCP23017_IODIRA     0x00
#define MCP23017_IODIRB     0x01
#define MCP23017_IPOLA      0x02
#define MCP23017_IPOLB      0x03
#define MCP23017_GPINTENA   0x04
#define MCP23017_GPINTENB   0x05
#define MCP23017_DEFVALA    0x06
#define MCP23017_DEFVALB    0x07
#define MCP23017_INTCONA    0x08
#define MCP23017_INTCONB    0x09
#define MCP23017_IOCON      0x0A
#define MCP23017_GPPUA      0x0C
#define MCP23017_GPPUB      0x0D
#define MCP23017_INTFA      0x0E
#define MCP23017_INTFB      0x0F
#define MCP23017_INTCAPA    0x10
#define MCP23017_INTCAPB    0x11
#define MCP23017_GPIOA      0x12
#define MCP23017_GPIOB      0x13
#define MCP23017_OLATA      0x14
#define MCP23017_OLATB      0x15

// INTCON register flags
#define MCP23017_IOCON_INTPOL  0x02
#define MCP23017_IOCON_ODR     0x04
#define MCP23017_IOCON_DISSLW  0x10
#define MCP23017_IOCON_SEQOP   0x20
#define MCP23017_IOCON_MIRROR  0x40
#define MCP23017_IOCON_BANK    0x80

#define MCP23017_INT_RISING   0
#define MCP23017_INT_FALLING  1
#define MCP23017_INT_BOTH     2

#define MCP23017_VALID_PIN(pin)  ( (pin) >= 0 && (pin) < 16 )
#define MCP23017_PIN_MASK        0x07
#define MCP23017_BANK_A(pin)     ( ((pin) & 0x08) ? false : true )
#define MCP23017_BANK_B(pin)     ( ((pin) & 0x08) ? true : false )


// @class MCPException
#ifndef MCP_EXCEPTION_CLASS
#define MCP_EXCEPTION_CLASS
#define MCP_ERROR_MSG_LEN 256

#include <stdarg.h>
#include <exception>

class MCPException : public std::exception {
public:
	//! Constructor
	/*! Constructor of the exception. Can format an error message.
	 *  @param f the format string (as in printf)
	 *  @param ... variable parameters, corresponding to the format string.
	 */
	MCPException(const char* f, ...) {
		va_list ap;
		va_start(ap, f);
		vsnprintf(_msg, MCP_ERROR_MSG_LEN, f, ap);
		va_end(ap);
	}
	MCPException(const char* f, va_list ap) {
		vsnprintf(_msg, MCP_ERROR_MSG_LEN, f, ap);
	}

	//! what(), return the error string
	virtual const char* what() const throw() {
		return _msg;
	}

private:
	char _msg[MCP_ERROR_MSG_LEN];
};
#else
class MCPException;
#endif


// @Class mcp23017
class mcp23017 {
public:
	// Constructor
	mcp23017(int i2c_bus, int address);

	// Destructor
	~mcp23017();

	// Copy constructor
	mcp23017(const mcp23017 &obj);

	// Assignment operator
	mcp23017& operator= (const mcp23017 &obj);

	/*
	 * Control interface
	 */
public:
	// Set GPIO direction
	void setDirection(int pin, bool isinput);

	// Set GPIO direction of multiple pins
	void setDirection(const std::vector<int>& pins, const std::vector<bool>& isinput);

	// Get GPIO direction
	bool getDirection(int pin);

	// Get GPIO direction of multiple pins
	std::vector<bool> getDirection(const std::vector<int>& pins);

	// Set GPIO polarity
	void setPolarity(int pin, bool inverted);

	// Set GPIO polarity of multiple pins
	void setPolarity(const std::vector<int>& pins, const std::vector<bool>& inverted);

	// Get GPIO polarity
	bool getPolarity(int pin);

	// Get GPIO polarity of multiple pins
	std::vector<bool> getPolarity(const std::vector<int>& pins);

	// Set GPIO pull-up
	void setPullup(int pin, bool enable);

	// Set GPIO pull-up of multiple pins
	void setPullup(const std::vector<int>& pins, const std::vector<bool>& enable);

	// Get GPIO pull-up
	bool getPullup(int pin);

	// Get GPIO pull-up of multiple pins
	std::vector<bool> getPullup(const std::vector<int>& pins);

	// Set interrupt on change
	void setInterrupt(int pin, int type);

	// Set interrupt on change of multiple pins
	void setInterrupt(const std::vector<int>& pins, const std::vector<int>& types);

	// Get interrupt status
	bool isInterruptEnabled(int pin);

	// Get interrupt status of multiple pin
	std::vector<bool> isInterruptEnabled(std::vector<int>& pins);

	// Clear interrupt on change
	void clearInterrupt(int pin);

	// Clear interrupt on change of multiple pins
	void clearInterrupt(const std::vector<int>& pins);

	// Get interrupt flags
	uint8_t getInterruptFlagsA() { return getInterruptFlags(0); }
	uint8_t getInterruptFlagsB() { return getInterruptFlags(1); }
	uint8_t getInterruptFlags(uint8_t bank);

	// Read GPIO
	bool readGPIO(int pin);

	// Read multiple GPIO
	std::vector<bool> readGPIO(const std::vector<int>& pins);

	// Write GPIO
	void writeGPIO(int pin, bool value);

	// Write multiple GPIO
	void writeGPIO(const std::vector<int>& pins, const std::vector<bool>& values);

	// Set configuration register
	void setConfiguration(uint8_t conf_set, uint8_t conf_clear);

	// Get configuration register
	uint8_t getConfiguration();

	// Reset configuration
	void reset();

	/*
	 * Raw access interface
	 */
public:
	// Check pin vectors
	bool checkPinVectors(const std::vector<int>& pins, const std::vector<bool>& values);

	// Set device register
	void setRegister(uint8_t reg, uint8_t value);

	// Get device register
	uint8_t getRegister(uint8_t reg);

	// Dump a sequence of configuration registers
	void dumpRegisters(uint8_t reg_start, uint8_t numreg, std::vector<uint8_t>& values);

private:
	int _addr;
	int _fd;
};

#endif
