#include <iostream>
#include <tango.h>
#include <stdexcept>
#include "../Common/endian_conversion.h"
#include <time.h>

// Macro to measure elapsed time in ms
#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif


int32_t read_parameter(uint8_t addr, uint16_t num);
int16_t write_parameter16(uint8_t addr, uint16_t num, uint16_t value);
int32_t write_parameter32(uint8_t addr, uint16_t num, int32_t value);

Tango::DeviceProxy* dev = NULL;

void msleep(int msec) {
	struct timespec st;
	st.tv_sec = msec / 1000;
	st.tv_nsec = (msec % 1000) * 10000000;
	nanosleep(&st, NULL);
}

int main(int argc, char *argv[]) {

	cout << "MAG turbopump address changer" << endl;
	if(argc < 2) {
		cout << "Usage: " << argv[0] << " <communication proxy>" << endl;
		return 0;
	}

	try {
		cout << "Connecting to " << argv[1] << "... ";
		dev = new Tango::DeviceProxy(argv[1]);
		cout << " OK!" << endl;
	} catch(Tango::DevFailed &e) {
		cout << " Failed! (Error: " << e.errors[0].desc << ")" << endl;
		return -1;
	}

	cout << "Scanning all addresses..." << endl;

/*
2 ADR Frequency converter address RS232: 0 RS485: 0...15
3-4 PKE Parameter number and type of access Value (s. 4.1)
5 – Reserved 0
6 IND Parameter index Value (s. 4.1)
7-10 PWE Parameter value 0 Value Value
11-12 PZD1 STW, ZSW Status and control bits Value (see 4.3 / 4.4)
13-14 PZD2, HSW HIW, (MSW) Current stator frequency (= P3) 0 0 Value (Hz)
15-16 PZD3, HSW Current frequency converter HIW, (LSW) temperature (= P11) 0 0 Value (°C)
17-18 PZD4 Current motor current (= P5) 0 0 Value (0.1 A)
19-20 PZD5 Current pump temperature (= P127) 0 0 Value (°C)
21-22 PZD6 Current intermediate circuit voltage (=P4) 0 0 Value (0.1 V)
*/

	// Map to hold pump addresses and serial numbers
	std::map<uint16_t, int32_t> pumps;

	for(uint16_t i = 0; i < 32; i++) {
		// Scan all addresses. Check if a pump is connected by asking the serial number
		try {
			int value = read_parameter(i, 52);
			cout << "Found pump at address " << i << " with serial " << value; // << endl;
			pumps[i] = value;
			
			value = read_parameter(i, 24);
			cout << " (setpoint: " << value << ")" << endl;

		} catch(std::exception &e) {
			// No pump connected
		}
	}
	
	if(pumps.empty()) {
		cout << "No pumps found on the bus. Exiting." << endl;
		return 0;
	}

	cout << "Chose the address you want to change: ";
	uint16_t address = 0;
	cin >> address;
	if(pumps.find(address) == pumps.end()) {
		cout << "Error: the supplied address (" << address << ") is not valid." << endl;
		return -1;
		
	} else {
		uint16_t curr_addr = 0;
		try {
			curr_addr = read_parameter(address, 254);
		} catch(std::exception &e) {
			cout << "Failed to read current address settings from pump. Error: " << e.what() << endl;
			return -1;
		}
		cout << "Changing address for pump with serial " << pumps[address] << " (current address: " << curr_addr << ")" << endl;
		cout << "Insert new address (0 to 31): ";
		uint16_t new_addr = 0;
		cin >> new_addr;
		if(pumps.find(new_addr) != pumps.end()) {
			cout << "Cannot use an address already in use by another pump." << endl;
			return -1;
		}
		if(new_addr >= 32) {
			cout << "Invalid address (" << new_addr << "). Pump address should be between 0 and 31." << endl;
			return -1;
		}
		
		// Confirm change
		cout << "Changing address of pump with serial " << pumps[address] << " from " << address << " to " << new_addr << ". Please confirm by pressing Y." << endl;
		char c = 'n';
		cin >> c;
		if(c == 'Y' || c == 'y') {
			try {
				uint16_t check = write_parameter16(address, 254, new_addr);
				if(check != new_addr) {
					cout << "The read-back address was not the one we wrote. Something went wrong." << endl;
					return -1;
				}
			} catch(std::exception &e) {
				cout << "Failed to write address to pump. Error: " << e.what() << endl;
				return -1;
			}
			
			// Send save command to make modification permanent
			try {
				write_parameter32(address, 8, 1);
			} catch(std::exception &e) {
				cout << "Failed to save modification. Error: " << e.what() << endl;
				return -1;
			}

// 			} else {
// 				sleep(1);
// 				// Try to read back the serial number from the new address
// 				try {
// 					int32_t serial = read_parameter(new_addr, 52);
// 					if(pumps[address] != serial) {
// 						cout << "The serial number we read from the new address (" << serial << ") does not match the old one (" << pumps[address] << "). Something went wrong." << endl;
// 						return -1;
// 					} else {
// 						cout << "Correctly change the address of pump with serial " << serial << endl;
// 					}
// 				} catch(std::exception &e) {
// 					cout << "Failed to read back serial number from new address. Error: " << e.what() << endl;
// 					return -1;
// 				}
// 			}

		} else {
			cout << "Aborted (code: " << c << ")" << endl;
			return -1;
		}
	}

	return 0;
}


int32_t read_parameter(uint8_t addr, uint16_t num) {

	// Allocate buffer
	Tango::DevVarCharArray* argin = new Tango::DevVarCharArray(21);
	argin->length(21);
	uint8_t *buffer = argin->get_buffer();

	// Prepare message
	memset(buffer, 0, 21);
	buffer[0] = addr;  // Address
	*((uint16_t*)(buffer+1)) = invert_byte_order((uint16_t)(0x1000 | (num & 0x07FF)));  // Parameter name and type of request
	buffer[4] = 0;  // Index
	*((uint16_t*)(buffer+9)) = 0;  // Control word

	try {
		Tango::DeviceData din;
		din << argin;

		Tango::DeviceData dout = dev->command_inout("SendTelegram", din);
		const Tango::DevVarCharArray* argout;
		if(dout >> argout) {
			const uint8_t *buffer = argout->get_buffer();
			uint16_t pke = invert_byte_order(*((uint16_t*)(buffer+1)));
			if(pke & 0x1000) {
				// Returned 16 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;
				
			} else if(pke & 0x2000) {
				// Returned 32 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;

			} else {
				// Failure
				throw std::runtime_error("Failure");
			}

		} else {
			throw std::runtime_error("Failed to parse response");
		}

	} catch(Tango::DevFailed &e) {
		std::stringstream msg;
		for(size_t i = 0; i < e.errors.length(); i++)
			msg << "[" << i << "] " << e.errors[i].desc << " (from: " << e.errors[i].origin << ")" << endl;
		throw std::runtime_error(msg.str());
	}
}

int16_t write_parameter16(uint8_t addr, uint16_t num, uint16_t value) {
	// Allocate buffer
	Tango::DevVarCharArray* argin = new Tango::DevVarCharArray(21);
	argin->length(21);
	uint8_t *buffer = argin->get_buffer();

	// Prepare message
	memset(buffer, 0, 21);
	buffer[0] = addr;  // Address
	*((uint16_t*)(buffer+1)) = invert_byte_order((uint16_t)(0x2000 | (num & 0x07FF)));  // Parameter name and type of request
	buffer[4] = 0;  // Index
	*((int32_t*)(buffer+5)) = invert_byte_order(int32_t(value)); // Shifted by 2 because is a 16 bit value in a 32 bit field
	*((uint16_t*)(buffer+9)) = 0;  // Control word

	try {
		Tango::DeviceData din;
		din << argin;

		Tango::DeviceData dout = dev->command_inout("SendTelegram", din);
		const Tango::DevVarCharArray* argout;
		if(dout >> argout) {
			const uint8_t *buffer = argout->get_buffer();
			uint16_t pke = invert_byte_order(*((uint16_t*)(buffer+1)));
			if(pke & 0x1000) {
				// Returned 16 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;
				
			} else if(pke & 0x2000) {
				// Returned 32 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;

			} else {
				// Failure
				throw std::runtime_error("Failure");
			}
			
		} else {
			throw std::runtime_error("Failed to parse response");
		}

	} catch(Tango::DevFailed &e) {
		throw std::runtime_error("Communication error");
	}
}

int32_t write_parameter32(uint8_t addr, uint16_t num, int32_t value) {
	// Allocate buffer
	Tango::DevVarCharArray* argin = new Tango::DevVarCharArray(21);
	argin->length(21);
	uint8_t *buffer = argin->get_buffer();

	// Prepare message
	memset(buffer, 0, 21);
	buffer[0] = addr;  // Address
	*((uint16_t*)(buffer+1)) = invert_byte_order((uint16_t)(0x3000 | (num & 0x07FF)));  // Parameter name and type of request
	buffer[4] = 0;  // Index
	*((int32_t*)(buffer+5)) = invert_byte_order(value);
	*((uint16_t*)(buffer+9)) = 0;  // Control word

	try {
		Tango::DeviceData din;
		din << argin;

		Tango::DeviceData dout = dev->command_inout("SendTelegram", din);
		const Tango::DevVarCharArray* argout;
		if(dout >> argout) {
			const uint8_t *buffer = argout->get_buffer();
			uint16_t pke = invert_byte_order(*((uint16_t*)(buffer+1)));
			if(pke & 0x1000) {
				// Returned 16 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;
				
			} else if(pke & 0x2000) {
				// Returned 32 bit value
				int32_t value = invert_byte_order(*((int32_t*)(buffer+5)));
				return value;

			} else {
				// Failure
				throw std::runtime_error("Failure");
			}
			
		} else {
			throw std::runtime_error("Failed to parse response");
		}

	} catch(Tango::DevFailed &e) {
		throw std::runtime_error("Communication error");
	}
	
}
