// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
#include <iostream>
#include <string>
#include <string.h>
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

void print_engine_settings(const engine_settings_t& engine) {
	int usteps = 0x1 << (engine.MicrostepMode - 1);

	#define ENGINE_REVERSE   		0x01	// Reverse flag.
	#define ENGINE_CURRENT_AS_RMS	0x02	// Engine current meaning flag.
	#define ENGINE_MAX_SPEED		0x04	// Max speed flag.
	#define ENGINE_ANTIPLAY			0x08	// Play compensation flag.
	#define ENGINE_ACCEL_ON			0x10	// Acceleration enable flag.
	#define ENGINE_LIMIT_VOLT		0x20	// Maximum motor voltage limit enable flag(is only used with DC motor).
	#define ENGINE_LIMIT_CURR		0x40	// Maximum motor current limit enable flag(is only used with DC motor).
	#define ENGINE_LIMIT_RPM		0x80	// Maximum motor speed limit enable flag.

	// Print engine settings
	cout << "Engine settings: " << endl;
	cout << "Rated voltage = " << double(engine.NomVoltage) / 10.0 << " V" << endl;
	cout << "Rated current = " << engine.NomCurrent << " mA" << endl;
	cout << "Nominal (max) speed = " << engine.NomSpeed << " (" << engine.uNomSpeed << "/" << usteps << ")" << endl;
	cout << "Engine Flags = ";
	if(engine.EngineFlags & ENGINE_REVERSE) {
		cout << "ENGINE_REVERSE | ";
	}
	if(engine.EngineFlags & ENGINE_CURRENT_AS_RMS) {
		cout << "ENGINE_CURRENT_AS_RMS | ";
	}
	if(engine.EngineFlags & ENGINE_MAX_SPEED) {
		cout << "ENGINE_MAX_SPEED | ";
	}
	if(engine.EngineFlags & ENGINE_ANTIPLAY) {
		cout << "ENGINE_ANTIPLAY | ";
	}
	if(engine.EngineFlags & ENGINE_ACCEL_ON) {
		cout << "ENGINE_ACCEL_ON | ";
	}
	if(engine.EngineFlags & ENGINE_LIMIT_VOLT) {
		cout << "ENGINE_LIMIT_VOLT | ";
	}
	if(engine.EngineFlags & ENGINE_LIMIT_CURR) {
		cout << "ENGINE_LIMIT_CURR | ";
	}
	if(engine.EngineFlags & ENGINE_LIMIT_RPM) {
		cout << "ENGINE_LIMIT_RPM | ";
	}
	cout << endl;

	cout << "Antiplay = " << engine.Antiplay << endl;
	cout << "Microstep Mode = 1/" << usteps << endl;
	cout << "Steps per revolution = " << engine.StepsPerRev << endl;
}


int main(int argc, char *argv[]) {

	if(argc < 2) {
		cout << "Need to specify serial number (all caps!)" << endl;
		return 0;
	}

	set_logging_callback(logging, NULL);

	// Format device URI
	std::string sn(argv[1]);
	std::transform(sn.begin(), sn.end(), sn.begin(), ::toupper);
	std::stringstream device_name;
	device_name << "xi-com:/dev/ximc/";
	for(size_t i = sn.length(); i < 8; i++)
		device_name << "0";
	device_name << sn;

	// Open device
	cout << "Opening device " << device_name.str() << endl;
	device_t dev = open_device(device_name.str().c_str());

	if(dev == device_undefined) {
		cout << "Failed to open device." << endl;
		exit(-1);
	}

	result_t r = 0;

	// Reload settings from flash
	cout << "Reloading settings from FLASH... ";
	r = command_read_settings(dev);
	if(r != result_ok) {
		cout << "Failed to read controller settings (Error: " << r << ")" << endl;
		close_device(&dev);
		return -1;
	}
	msec_sleep(100);
	cout << "Done!" << endl;

	// Get engine settings
	engine_settings_t engine;
	memset(&engine, 0, sizeof(engine_settings_t));
	r = get_engine_settings(dev, &engine);
	if(r != result_ok) {
		cout << "Failed to read engine settings (Error: " << r << ")" << endl;
		close_device(&dev);
		return -1;
	}

	print_engine_settings(engine);

	cout << "Update nominal speed? [y/n]" << endl;
	string str;
	cin >> str;

	if(str[0] == 'y' || str[0] == 'Y') {
		int usteps = 0x1 << (engine.MicrostepMode - 1);
		int new_s = -1, new_us = -1;
		cout << "Enter new maximum speed:" << endl;
		while(true) {
			cout << "[steps] = ";
			cin >> new_s;
			if(new_s < 0)
				cout << "Invalid value";
			else
				break;
		}
		while(true) {
			cout << "[microsteps 1/" << usteps << "] = ";
			cin >> new_us;
			if(new_s < 0 || new_us > 256)
				cout << "Invalid value!" << endl;
			else
				break;
		}
		cout << "New maximum speed: " << new_s << " (" << new_us << "/" << usteps << "). Confirm? [y/n]" << endl;
		cin >> str;
		if(str[0] == 'y' || str[0] == 'Y') {
			engine.NomSpeed = new_s;
			engine.uNomSpeed = new_us;
			r = set_engine_settings(dev, &engine);
			if(r != result_ok) {
				cout << "Failed to set engine settings (Error: " << r << ")" << endl;
				close_device(&dev);
				return -1;
			}

			msec_sleep(500);
			memset(&engine, 0, sizeof(engine_settings_t));
			r = get_engine_settings(dev, &engine);
			if(r != result_ok) {
				cout << "Failed to read engine settings (Error: " << r << ")" << endl;
				close_device(&dev);
				return -1;
			}
			print_engine_settings(engine);

			// Save settings to FLASH
			cout << "Saving settings to FLASH...";
			r = command_save_settings(dev);
			if(r != result_ok) {
				cout << "Failed to save controller settings (Error: " << r << ")" << endl;
				close_device(&dev);
				return -1;
			}
			msec_sleep(100);
			cout << "Done!" << endl;
		}
	}

	close_device(&dev);
	return 0;
}
