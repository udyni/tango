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

#include "mcp23017.h"


mcp23017::mcp23017(int bus, int address) :
	_fd(-1),
	_addr(address)
{

	// Format adapter name
	char filename[20];
	::snprintf(filename, 19, "/dev/i2c-%d", bus);

	// Open adapter
	_fd = ::open(filename, O_RDWR);
	if(_fd < 0) {
		throw MCPException("Failed to open i2c adapter '%s' (Error: %s)", filename, strerror(errno));
	}

	// Configure device address
	if(ioctl(_fd, I2C_SLAVE, _addr) < 0) {
		throw MCPException("Failed to configure device addess 0x%02X (Error: %s)", _addr, strerror(errno));
	}
#ifdef _DEBUG
	printf("Set slave address to 0x%02X\n", address);
#endif

	// Check adapter functionality
	unsigned long funcs = 0;
	if(ioctl(_fd, I2C_FUNCS, &funcs) < 0) {
		throw MCPException("Failed to read adapter functionality (Error: %s)", _addr, strerror(errno));
	}

#ifdef _DEBUG
	// Print adapter functionality
	printf("Adapter %d on device file %s:\n", bus, filename);
	printf("Adapter functions flags: 0x%08lX\n", funcs);
	if(funcs & I2C_FUNC_I2C)
		printf("Adapter support I2C_FUNC_I2C\n");
	if(funcs & I2C_FUNC_10BIT_ADDR)
		printf("Adapter support I2C_FUNC_10BIT_ADDR\n");
	if(funcs & I2C_FUNC_PROTOCOL_MANGLING)
		printf("Adapter support I2C_FUNC_PROTOCOL_MANGLING\n");
	if(funcs & I2C_FUNC_SMBUS_PEC)
		printf("Adapter support I2C_FUNC_SMBUS_PEC\n");
	if(funcs & I2C_FUNC_NOSTART)
		printf("Adapter support I2C_FUNC_NOSTART\n");
	if(funcs & I2C_FUNC_SMBUS_BLOCK_PROC_CALL)
		printf("Adapter support I2C_FUNC_SMBUS_BLOCK_PROC_CALL\n");
	if(funcs & I2C_FUNC_SMBUS_QUICK)
		printf("Adapter support I2C_FUNC_SMBUS_QUICK\n");
	if(funcs & I2C_FUNC_SMBUS_READ_BYTE)
		printf("Adapter support I2C_FUNC_SMBUS_READ_BYTE\n");
	if(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)
		printf("Adapter support I2C_FUNC_SMBUS_WRITE_BYTE\n");
	if(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_READ_BYTE_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_WRITE_BYTE_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_READ_WORD_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_WRITE_WORD_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_PROC_CALL)
		printf("Adapter support I2C_FUNC_SMBUS_PROC_CALL\n");
	if(funcs & I2C_FUNC_SMBUS_READ_BLOCK_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_READ_BLOCK_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
		printf("Adapter support I2C_FUNC_SMBUS_WRITE_BLOCK_DATA\n");
	if(funcs & I2C_FUNC_SMBUS_READ_I2C_BLOCK)
		printf("Adapter support I2C_FUNC_SMBUS_READ_I2C_BLOCK\n");
	if(funcs & I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)
		printf("Adapter support I2C_FUNC_SMBUS_WRITE_I2C_BLOCK\n");
#endif

	// Check availability of I2C_FUNC_I2C
	if( !(funcs & I2C_FUNC_I2C) )
		throw MCPException("I2C adapter does not support I2C transactions");
}

// Destructor
mcp23017::~mcp23017() {
	// Close file
	if(_fd != -1)
		::close(_fd);
}

// Copy constructor
mcp23017::mcp23017(const mcp23017 &obj) {
	_fd = dup(obj._fd);
	if(_fd < 0) {
		throw MCPException("Failed to duplicate i2c adapter handle (Error: %s)", strerror(errno));
	}
	_addr = obj._addr;
}

// Assignment operator
mcp23017& mcp23017::operator= (const mcp23017 &obj) {
	if(_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
	_fd = ::dup(obj._fd);
	if(_fd < 0) {
		throw MCPException("Failed to duplicate i2c adapter handle (Error: %s)", strerror(errno));
	}
	_addr = obj._addr;
}

// Write register value
void mcp23017::setRegister(uint8_t reg, uint8_t value) {
	// Format I2C message
	struct i2c_msg messages[1];
	uint8_t outbuf[2];
	messages[0].addr = _addr;
	messages[0].flags = 0;
	messages[0].len = sizeof(outbuf);
	messages[0].buf = outbuf;
	outbuf[0] = reg;
	outbuf[1] = value;

	// Send message
	struct i2c_rdwr_ioctl_data packets;
	packets.msgs  = messages;
	packets.nmsgs = 1;
	if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
		throw MCPException("Failed to set device register 0x%02X (Error: %s)", reg, strerror(errno));
	}
}

// Read register value
uint8_t mcp23017::getRegister(uint8_t reg) {
	// Format I2C message
	struct i2c_msg messages[2];
	uint8_t outbuf, inbuf;
	messages[0].addr = _addr;
	messages[0].flags = 0;
	messages[0].len = sizeof(outbuf);
	messages[0].buf = &outbuf;
	outbuf = reg;
	messages[1].addr = _addr;
	messages[1].flags = I2C_M_RD;
	messages[1].len = sizeof(inbuf);
	messages[1].buf = &inbuf;

	// Send message
	struct i2c_rdwr_ioctl_data packets;
	packets.msgs  = messages;
	packets.nmsgs = 2;
	if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
		throw MCPException("Failed to get device register 0x%02X (Error: %s)", reg, strerror(errno));
	}
	return inbuf;
}

// Dump registers
void mcp23017::dumpRegisters(uint8_t reg_start, uint8_t numreg, std::vector<uint8_t>& values) {
	// Clear output values register
	values.clear();

	// Check the number of registers to read
	if(numreg == 0)
		return;

	// Get control register
	uint8_t intcon = getRegister(MCP23017_IOCON);

	// Update control register for sequential read
	if(!(intcon & MCP23017_IOCON_SEQOP))
		setRegister(MCP23017_IOCON, intcon & MCP23017_IOCON_SEQOP);

	// Format messages for sequential read
	struct i2c_msg messages[2];
	uint8_t outbuf;
	messages[0].addr = _addr;
	messages[0].flags = 0;
	messages[0].len = sizeof(outbuf);
	messages[0].buf = &outbuf;
	outbuf = reg_start;
	messages[1].addr = _addr;
	messages[1].flags = I2C_M_RD;
	messages[1].len = numreg;
	messages[1].buf = new uint8_t[numreg];

	try {
		// Send message
		struct i2c_rdwr_ioctl_data packets;
		packets.msgs  = messages;
		packets.nmsgs = 2;
		if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
			throw MCPException("Failed to get dump device registers starting from 0x%02X for %d registers (Error: %s)", reg_start, int(numreg), strerror(errno));
		}
	} catch(MCPException &e) {
		// IOCTL failed
		delete[] messages[1].buf;
		if(!(intcon & MCP23017_IOCON_SEQOP))
			setRegister(MCP23017_IOCON, intcon);
		throw e;
	}
	// Restore INTCON if needed
	if(!(intcon & MCP23017_IOCON_SEQOP))
			setRegister(MCP23017_IOCON, intcon);

	// Return read values
	for(size_t i = 0; i < numreg; i++)
		values.push_back(messages[1].buf[i]);
}

// Set GPIO direction
void mcp23017::setDirection(int pin, bool isinput) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_IODIRA);
	else // Bank B
		reg = getRegister(MCP23017_IODIRB);

	if(isinput)
		reg |= (0x1 << (pin & MCP23017_PIN_MASK));
	else
		reg &= ~(0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) // Bank A
		setRegister(MCP23017_IODIRA, reg);
	else // Bank B
		setRegister(MCP23017_IODIRB, reg);
}

// Set GPIO direction of multiple pins
void mcp23017::setDirection(const std::vector<int>& pins, const std::vector<bool>& isinput) {
	// Pins' number must be equal to values' number
	if(pins.size() != isinput.size())
		throw MCPException("The number of pins and the number of values must be the same");
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_IODIRA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_IODIRB);

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			if(isinput[i])
				reg_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			if(isinput[i])
				reg_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	// Write updated registers
	if(need_bank_a)
		setRegister(MCP23017_IODIRA, reg_a);
	if(need_bank_b)
		setRegister(MCP23017_IODIRB, reg_b);
}

// Get GPIO direction
bool mcp23017::getDirection(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_IODIRA);
	else // Bank B
		reg = getRegister(MCP23017_IODIRB);

	return reg & (0x1 << (pin & MCP23017_PIN_MASK));
}

// Get GPIO direction of multiple pins
std::vector<bool> mcp23017::getDirection(const std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_IODIRA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_IODIRB);

	// Get values
	std::vector<bool> out;
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i]))
			out.push_back(reg_a & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
		else
			out.push_back(reg_b & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
	}
	return out;
}

// Set GPIO polarity
void mcp23017::setPolarity(int pin, bool inverted) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_IPOLA);
	else // Bank B
		reg = getRegister(MCP23017_IPOLB);

	if(inverted)
		reg |= (0x1 << (pin & MCP23017_PIN_MASK));
	else
		reg &= ~(0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) // Bank A
		setRegister(MCP23017_IPOLA, reg);
	else // Bank B
		setRegister(MCP23017_IPOLB, reg);
}

// Set GPIO polarity of multiple pins
void mcp23017::setPolarity(const std::vector<int>& pins, const std::vector<bool>& inverted) {
	// Pins' number must be equal to values' number
	if(pins.size() != inverted.size())
		throw MCPException("The number of pins and the number of values must be the same");
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_IPOLA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_IPOLB);

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			if(inverted[i])
				reg_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			if(inverted[i])
				reg_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	// Write updated registers
	if(need_bank_a)
		setRegister(MCP23017_IPOLA, reg_a);
	if(need_bank_b)
		setRegister(MCP23017_IPOLB, reg_b);
}

// Get GPIO polarity
bool mcp23017::getPolarity(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_IPOLA);
	else // Bank B
		reg = getRegister(MCP23017_IPOLB);

	return reg & (0x1 << (pin & MCP23017_PIN_MASK));
}

// Get GPIO polarity of multiple pins
std::vector<bool> mcp23017::getPolarity(const std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_IPOLA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_IPOLB);

	// Get values
	std::vector<bool> out;
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i]))
			out.push_back(reg_a & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
		else
			out.push_back(reg_b & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
	}
	return out;
}

// Set GPIO pull-up
void mcp23017::setPullup(int pin, bool enable) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_GPPUA);
	else // Bank B
		reg = getRegister(MCP23017_GPPUB);

	if(enable)
		reg |= (0x1 << (pin & MCP23017_PIN_MASK));
	else
		reg &= ~(0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) // Bank A
		setRegister(MCP23017_GPPUA, reg);
	else // Bank B
		setRegister(MCP23017_GPPUB, reg);
}

// Set GPIO pull-up of multiple pins
void mcp23017::setPullup(const std::vector<int>& pins, const std::vector<bool>& enable) {
	// Pins' number must be equal to values' number
	if(pins.size() != enable.size())
		throw MCPException("The number of pins and the number of values must be the same");
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_GPPUA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_GPPUB);

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			if(enable[i])
				reg_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			if(enable[i])
				reg_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	// Write updated registers
	if(need_bank_a)
		setRegister(MCP23017_GPPUA, reg_a);
	if(need_bank_b)
		setRegister(MCP23017_GPPUB, reg_b);
}

// Get GPIO pull-up
bool mcp23017::getPullup(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_GPPUA);
	else // Bank B
		reg = getRegister(MCP23017_GPPUB);

	return reg & (0x1 << (pin & MCP23017_PIN_MASK));
}

// Get GPIO pull-up of multiple pins
std::vector<bool> mcp23017::getPullup(const std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_GPPUA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_GPPUB);

	// Get values
	std::vector<bool> out;
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i]))
			out.push_back(reg_a & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
		else
			out.push_back(reg_b & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
	}
	return out;
}


// Set interrupt on change
void mcp23017::setInterrupt(int pin, int type) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg_en = 0;
	uint8_t reg_def = 0;
	uint8_t reg_con = 0;
	if(MCP23017_BANK_A(pin)) { // Bank A
		reg_en = getRegister(MCP23017_GPINTENA);
		reg_def = getRegister(MCP23017_DEFVALA);
		reg_con = getRegister(MCP23017_INTCONA);
	} else { // Bank B
		reg_en = getRegister(MCP23017_GPINTENB);
		reg_def = getRegister(MCP23017_DEFVALB);
		reg_con = getRegister(MCP23017_INTCONB);
	}

	switch(type) {
		case MCP23017_INT_RISING:
			// Default value 0, control bit 0
			reg_def &= ~(0x1 << (pin & MCP23017_PIN_MASK));
			reg_con |= (0x1 << (pin & MCP23017_PIN_MASK));
			break;
		case MCP23017_INT_FALLING:
			// Default value 1, control bit 0
			reg_def |= (0x1 << (pin & MCP23017_PIN_MASK));
			reg_con |= (0x1 << (pin & MCP23017_PIN_MASK));
			break;
		case MCP23017_INT_BOTH:
			// Control bit 1
			reg_con &= ~(0x1 << (pin & MCP23017_PIN_MASK));
			break;
		default:
			throw MCPException("Invalid interrupt mode (%d)", type);
	}
	reg_en |= (0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) { // Bank A
		setRegister(MCP23017_GPINTENA, reg_en);
		setRegister(MCP23017_DEFVALA, reg_def);
		setRegister(MCP23017_INTCONA, reg_con);
	} else { // Bank B
		setRegister(MCP23017_GPINTENB, reg_en);
		setRegister(MCP23017_DEFVALB, reg_def);
		setRegister(MCP23017_INTCONB, reg_con);
	}
}

// Set interrupt on change of multiple pins
void mcp23017::setInterrupt(const std::vector<int>& pins, const std::vector<int>& types) {
	// Pins' number must be equal to values' number
	if(pins.size() != types.size())
		throw MCPException("The number of pins and the number of values must be the same");
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	uint8_t reg_en_a = 0;
	uint8_t reg_def_a = 0;
	uint8_t reg_con_a = 0;
	uint8_t reg_en_b = 0;
	uint8_t reg_def_b = 0;
	uint8_t reg_con_b = 0;
	if(need_bank_a) {
		reg_en_a = getRegister(MCP23017_GPINTENA);
		reg_def_a = getRegister(MCP23017_DEFVALA);
		reg_con_a = getRegister(MCP23017_INTCONA);
	}
	if(need_bank_b) {
		reg_en_b = getRegister(MCP23017_GPINTENB);
		reg_def_b = getRegister(MCP23017_DEFVALB);
		reg_con_b = getRegister(MCP23017_INTCONB);
	}

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			switch(types[i]) {
				case MCP23017_INT_RISING:
					// Default value 0, control bit 0
					reg_def_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
					reg_con_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				case MCP23017_INT_FALLING:
					// Default value 1, control bit 0
					reg_def_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					reg_con_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				case MCP23017_INT_BOTH:
					// Control bit 1
					reg_con_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				default:
					throw MCPException("Invalid interrupt mode (%d) on pin %d", types[i], pins[i]);
			}
			reg_en_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			switch(types[i]) {
				case MCP23017_INT_RISING:
					// Default value 0, control bit 1
					reg_def_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
					reg_con_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				case MCP23017_INT_FALLING:
					// Default value 1, control bit 1
					reg_def_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					reg_con_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				case MCP23017_INT_BOTH:
					// Control bit 0
					reg_con_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
					break;
				default:
					throw MCPException("Invalid interrupt mode (%d) on pin %d", types[i], pins[i]);
			}
			reg_en_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	if(need_bank_a) {
		setRegister(MCP23017_GPINTENA, reg_en_a);
		setRegister(MCP23017_DEFVALA, reg_def_a);
		setRegister(MCP23017_INTCONA, reg_con_a);
	}
	if(need_bank_b) {
		setRegister(MCP23017_GPINTENB, reg_en_b);
		setRegister(MCP23017_DEFVALB, reg_def_b);
		setRegister(MCP23017_INTCONB, reg_con_b);
	}
}

// Get interrupt status
bool mcp23017::isInterruptEnabled(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_GPINTENA);
	else // Bank B
		reg = getRegister(MCP23017_GPINTENB);

	return reg & (0x1 << (pin & MCP23017_PIN_MASK));
}

// Get interrupt status of multiple pin
std::vector<bool> mcp23017::isInterruptEnabled(std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_GPINTENA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_GPINTENB);

	// Get values
	std::vector<bool> out;
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i]))
			out.push_back(reg_a & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
		else
			out.push_back(reg_b & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
	}
	return out;
}

// Clear interrupt on change
void mcp23017::clearInterrupt(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_GPINTENA);
	else // Bank B
		reg = getRegister(MCP23017_GPINTENB);

	// Clear interrupt
	reg &= ~(0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) // Bank A
		setRegister(MCP23017_GPINTENA, reg);
	else // Bank B
		setRegister(MCP23017_GPINTENB, reg);
}

// Clear interrupt on change of multiple pins
void mcp23017::clearInterrupt(const std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_GPINTENA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_GPINTENB);

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			reg_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			reg_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	// Write updated registers
	if(need_bank_a)
		setRegister(MCP23017_GPINTENA, reg_a);
	if(need_bank_b)
		setRegister(MCP23017_GPINTENB, reg_b);
}

// Get interrupt flags
uint8_t mcp23017::getInterruptFlags(uint8_t bank) {
	if(bank == 0)
		return getRegister(MCP23017_INTFA);
	else if(bank == 1)
		return getRegister(MCP23017_INTFB);
	else
		throw MCPException("The requested bank number (%d) is not valid", int(bank));
}

// Read GPIO
bool mcp23017::readGPIO(int pin) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_GPIOA);
	else // Bank B
		reg = getRegister(MCP23017_GPIOB);

	return reg & (0x1 << (pin & MCP23017_PIN_MASK));
}

// Read multiple GPIO
std::vector<bool> mcp23017::readGPIO(const std::vector<int>& pins) {
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_GPIOA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_GPIOB);

	// Get values
	std::vector<bool> out;
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i]))
			out.push_back(reg_a & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
		else
			out.push_back(reg_b & (0x1 << (pins[i] & MCP23017_PIN_MASK)));
	}
	return out;
}

// Write GPIO
void mcp23017::writeGPIO(int pin, bool value) {
	if(!MCP23017_VALID_PIN(pin))
		throw MCPException("The given pin number (%d) is not valid", pin);

	uint8_t reg = 0;
	if(MCP23017_BANK_A(pin)) // Bank A
		reg = getRegister(MCP23017_OLATA);
	else // Bank B
		reg = getRegister(MCP23017_OLATB);

	if(value)
		reg |= (0x1 << (pin & MCP23017_PIN_MASK));
	else
		reg &= ~(0x1 << (pin & MCP23017_PIN_MASK));

	if(MCP23017_BANK_A(pin)) // Bank A
		setRegister(MCP23017_OLATA, reg);
	else // Bank B
		setRegister(MCP23017_OLATB, reg);
}

// Write multiple GPIO
void mcp23017::writeGPIO(const std::vector<int>& pins, const std::vector<bool>& values) {
	// Pins' number must be equal to values' number
	if(pins.size() != values.size())
		throw MCPException("The number of pins and the number of values must be the same");
	// Check that all requested pins are valid
	// Check also which banks we need
	bool need_bank_a = false;
	bool need_bank_b = false;
	for(size_t i = 0; i < pins.size(); i++) {
		if(!MCP23017_VALID_PIN(pins[i]))
			throw MCPException("The given pin number (%d) is not valid", pins[i]);
		if(MCP23017_BANK_A(pins[i]))
			need_bank_a = true;
		else
			need_bank_b = true;
	}

	// Get original registers
	uint8_t reg_a = 0;
	if(need_bank_a)
		reg_a = getRegister(MCP23017_OLATA);
	uint8_t reg_b = 0;
	if(need_bank_b)
		reg_b = getRegister(MCP23017_OLATB);

	// Update values
	for(size_t i = 0; i < pins.size(); i++) {
		if(MCP23017_BANK_A(pins[i])) {
			if(values[i])
				reg_a |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_a &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		} else {
			if(values[i])
				reg_b |= (0x1 << (pins[i] & MCP23017_PIN_MASK));
			else
				reg_b &= ~(0x1 << (pins[i] & MCP23017_PIN_MASK));
		}
	}

	// Write updated registers
	if(need_bank_a)
		setRegister(MCP23017_OLATA, reg_a);
	if(need_bank_b)
		setRegister(MCP23017_OLATB, reg_b);
}

// Set configuration register
void mcp23017::setConfiguration(uint8_t conf_set, uint8_t conf_clear) {

	uint8_t conf = getConfiguration();

	// Mask options
	uint8_t all_options = MCP23017_IOCON_INTPOL &
	                      MCP23017_IOCON_ODR &
	                      MCP23017_IOCON_DISSLW &
	                      MCP23017_IOCON_SEQOP &
	                      MCP23017_IOCON_MIRROR &
	                      MCP23017_IOCON_BANK;

	// Set bits
	conf |= (conf_set & all_options);

	// Clear bits
	conf &= ~(conf_clear & all_options);

	setRegister(MCP23017_IOCON, conf);
}

// Get configuration register
uint8_t mcp23017::getConfiguration() {
	return getRegister(MCP23017_IOCON);
}

// Reset configuration (equivalent to a hardware reset)
void mcp23017::reset() {
	try { setRegister(MCP23017_IODIRA,   0xFF); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_IODIRB,   0xFF); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_IPOLA,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_IPOLB,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPINTENA, 0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPINTENB, 0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_DEFVALA,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_DEFVALB,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTCONA,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTCONB,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_IOCON,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPPUA,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPPUB,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTFA,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTFB,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTCAPA,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_INTCAPB,  0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPIOA,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_GPIOB,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_OLATA,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
	try { setRegister(MCP23017_OLATB,    0x00); } catch(MCPException &e) { printf("Error: %s\n", e.what()); }
}
