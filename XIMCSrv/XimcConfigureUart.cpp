// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <codecvt>
#include <ximc.h>

using namespace std;


void logging(int loglevel, const wchar_t *message, void *user_data) {
	// Convert message to ASCII
	using convert_str = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_str, wchar_t> converter;
	cout << "[libximc] " << converter.to_bytes(message) << endl;
}

int main(int argc, char *argv[]) {

	// Get serial number from command line
	if(argc < 2) {
		cout << "Usage: XimcConfigureUart <serial>" << endl;
		exit(0);
	}

	// Configure logging callback
	set_logging_callback(logging, NULL);

	// Format device URI
	std::string sn(argv[1]);
	std::transform(sn.begin(), sn.end(), sn.begin(), ::toupper);
	std::stringstream device_name;
	device_name << "xi-com:/dev/ximc/";
	for(size_t i = sn.length(); i < 8; i++)
		device_name << "0";
	device_name << sn;

	// Open dev
	cout << "Opening device " << device_name.str() << endl;
 	device_t dev = open_device(device_name.str().c_str());
	if(dev == device_undefined) {
		cout << "Failed to open device" << endl;
		exit(-1);
	}

	result_t r = result_ok;

	cout << "Press [1] to check UART settings, press [2] to update UART settings:" << endl;
	char selection = (unsigned char)getchar();

	if(selection == '1') {
		// Read UART settings
		uart_settings_t uart;
		r = get_uart_settings(dev, &uart);
		if(r != result_ok) {
			cout << "Failed to read UART settings (Error code: " << r << ")" << endl;
			close_device(&dev);
			exit(-1);
		}
		cout << "Baud rate: " << uart.Speed << endl;
		cout << "Flags: ";
		if(uart.UARTSetupFlags & UART_PARITY_BIT_USE) {
			if(uart.UARTSetupFlags & UART_PARITY_BIT_ODD) {
				cout << "parity odd, ";
			} else {
				cout << "parity even, ";
			}
		} else {
			cout << "no parity, ";
		}
		if(uart.UARTSetupFlags & UART_STOP_BIT) {
			cout << "1 stop bits." << endl;
		} else {
			cout << "2 stop bits." << endl;
		}

	} else if(selection == '2') {
		// Reload settings from flash (to avoid saving anything other that UART settings)
		r = command_read_settings(dev);
		if(r != result_ok) {
			cout << "Failed to read settings from flash (Error code: " << r << ")" << endl;
			close_device(&dev);
			exit(-1);
		}

		// Configure UART for 115200 baud, parity even, 1 stop bit
		uart_settings_t uart;
		uart.Speed = 115200;
		uart.UARTSetupFlags = UART_PARITY_BIT_USE | UART_STOP_BIT;
		r = set_uart_settings(dev, &uart);
		if(r != result_ok) {
			cout << "Failed to configure UART (Error code: " << r << ")" << endl;
			close_device(&dev);
			exit(-1);
		}

		uart_settings_t new_uart;
		r = get_uart_settings(dev, &new_uart);
		if(r != result_ok) {
			cout << "Failed to check UART settings (Error code: " << r << ")" << endl;
			close_device(&dev);
			exit(-1);
		}

		if(new_uart.Speed != uart.Speed || new_uart.UARTSetupFlags != uart.UARTSetupFlags) {
			cout << "Failed to configure UART (Value mismatch)" << endl;
			close_device(&dev);
			exit(-1);
		}

		// Save settings
		r = command_save_settings(dev);
		if(r != result_ok) {
			cout << "Failed to save configuration to flash (Error code: " << r << ")" << endl;
			close_device(&dev);
			exit(-1);
		}
		cout << "Configuration done!" << endl;

	} else {
		cout << "Bad selection. Nothing to do." << endl;
	}
	close_device(&dev);
	return 0;
}
