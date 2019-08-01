/*
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <mcp23017.h>

using namespace std;


void check_device(int bus, int address) {
	try {
		mcp23017 dev(bus, address); 
	
		uint8_t conf = dev.getConfiguration();
		cout << "Chip configuration: 0x" << hex << int(conf) << dec << endl;
		uint8_t reg_a = dev.getRegister(MCP23017_GPIOA);
		uint8_t reg_b = dev.getRegister(MCP23017_GPIOB);
		cout << "Chip output A: 0x" << hex << int(reg_a) << dec << endl;
		cout << "Chip output B: 0x" << hex << int(reg_b) << dec << endl;

	} catch(const MCPException& e) {
		cout << "Failed to communicate with chip at address " << address << " (Error was: " << e.what() << ")" << endl;
	}
}


int main(int argc, char *argv[]) {

	if(argc < 2) {
		cout << "Usage:" << endl;
		cout << argv[0] << " <bus number> [<address to check>]" << endl;
		return 0;
	}

	// Parse address
	int bus = -1;
	try {
		bus = stoi(argv[1]);

	} catch(const std::invalid_argument& e) {
		cout << "Bad bus number (Error was: " << e.what() << ")" << endl;
		return -1;
	}

	if(argc > 2) {
		// Device address has been supplied
		int address = -1;
		try {
			address = stoi(argv[2]);
		} catch(const std::invalid_argument& e) {
			cout << "Bad address number (Error was: " << e.what() << ")" << endl;
			return -1;
		}

		check_device(bus, address);

	} else {
		// Scan all possible addresses
		for(int i = 32; i < 40; i++) {
			cout << "Checking for a device at address " << i << endl;
			check_device(bus, i);
		}
	}
}
