// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
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

#ifndef XIMC_PROTOCOL_H
#define XIMC_PROTOCOL_H

#include <stdint.h>
#include <string.h>

// Macros
#define SERIALIZE_VAR(buffer, offset, type, var) (*(type*)((buffer)+(offset))) = (var); (offset) += (sizeof(type))
#define UNSERIALIZE_VAR(buffer, offset, type, var) (var) = (*(type*)((buffer)+(offset))); (offset) += (sizeof(type))


// Supported XIMC commands
#define XIMC_STOP     0x706F7473   // Stop all movement (no payload, no response)
#define XIMC_PWOF     0x666F7770   // Turn off motor power (no payload, no response)
#define XIMC_MOVE     0x65766F6D   // Move absolute (position_t payload, no response)
#define XIMC_MOVR     0x72766F6D   // Move relative (position_t payload, no response)
#define XIMC_HOME     0x656D6F68   // Go home (no payload, no response)
#define XIMC_LEFT     0x7466656C   // Start a continuous move to the left (no payload, no response)
#define XIMC_RIGT     0x74676972   // Start a continuous move to the right (no payload, no response)
#define XIMC_SSTP     0x70747373   // Soft stop (no payload, no response)
#define XIMC_GPOS     0x736F7067   // Get position (no payload, gposition_t response)
#define XIMC_SPOS     0x736F7073   // Set current position (sposition_t payload, no response)
#define XIMC_ZERO     0x6F72657A   // Set current position as zero (no payload, no response)
#define XIMC_READ     0x64616572   // Read settings from flash (no payload, no response)
#define XIMC_GETS     0x73746567   // Get controller status (no payload, status_t response)
#define XIMC_GETC     0x63746567   // Return device electrical parameters, useful for charts (no payload, chart_data_t response)
#define XIMC_GETI     0x69746567   // Return device information (no payload, device_information_t response)
#define XIMC_GSER     0x72657367   // Read device serial number (no payload, int32_t response)
#define XIMC_GFWV     0x76776667   // Read controller's firmware version (no payload, 2x int8 + 1 int16 response)

// Configuration commands
#define XIMC_SFBS     0x73626673   // Set feedback settings
#define XIMC_GFBS     0x73626667   // Get feedback settings
#define XIMC_SMOV     0x766F6D73   // Configure setup movement (speed, acceleration, threshold and etc)
#define XIMC_GMOV     0x766F6D67   // Read setup movement (speed, acceleration, threshold and etc)
#define XIMC_SENG     0x676E6573   // Set engine settings
#define XIMC_GENG     0x676E6567   // Read engine settings
#define XIMC_SHOM     0x6D6F6873   // Set home settings
#define XIMC_GHOM     0x6D6F6867   // Read home settings

// Error codes
#define XIMC_ERRC     0x63727265   // Command not recognized
#define XIMC_ERRD     0x64727265   // CRC error
#define XIMC_ERRV     0x76727265   // Value error

typedef struct _position_t {
	int32_t position;      // Position in full steps
	int16_t uposition;     // Fractional position in usteps (-255 .. 255)
	uint8_t reserved[6];
	uint16_t checksum;     // Checksum
} position_t;
#define POSITION_T_LEN  (sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t) * 6 + sizeof(uint16_t))

typedef struct _gposition_t {
	int32_t position;      // Position in full steps
	int16_t uposition;     // Fractional position in usteps (-255 .. 255)
	int64_t enc_position;  // Encoder position
	uint8_t reserved[6];
	uint16_t checksum;     // Checksum
} gposition_t;
#define GPOSITION_T_LEN  (sizeof(uint32_t) + sizeof(uint16_t) + sizeof(int64_t) + sizeof(uint8_t) * 6 + sizeof(uint16_t))

typedef struct _sposition_t {
	int32_t position;      // Position in full steps
	int16_t uposition;     // Fractional position in usteps (-255 .. 255)
	int64_t enc_position;  // Encoder position
	uint8_t flags;         // Flags: SETPOS_IGNORE_POSITION or SETPOS_IGNORE_ENCODER
	uint8_t reserved[5];
	uint16_t checksum;     // Checksum
} sposition_t;
#define SETPOS_IGNORE_POSITION  0x1   // Ignore position
#define SETPOS_IGNORE_ENCODER   0x2   // Ignore encoder position
#define SPOSITION_T_LEN  (sizeof(uint32_t) + sizeof(uint16_t) + sizeof(int64_t) + sizeof(uint8_t) * 6 + sizeof(uint16_t))

typedef struct _status_t {
	uint8_t move_sts;      // Move status
	uint8_t mcmd_sts;      // Move command status
	uint8_t pwr_sts;       // Power status
	uint8_t enc_sts;       // Encoder status
	uint8_t wind_sts;      // Windings status
	int32_t position;      // Current position
	int16_t uposition;     // Current position (1/256 step)
	int64_t enc_position;  // Encoder position
	int32_t speed;         // Current speed
	int16_t uspeed;        // Current speed (1/256 step)
	int16_t i_pwr;         // Engine current
	int16_t v_pwr;         // Engine supply voltage
	int16_t i_usb;         // USB current
	int16_t v_usb;         // USB voltage
	int16_t temp;          // Temperature 0.1°C
	uint32_t flags;        // Set of flags specify motor shaft movement algorithm and list of limitations.
	uint32_t gpio_flags;   // Set of flags of gpio states
	uint8_t buff_free;     // This field shows the amount of free cells buffer synchronization chain.
	uint8_t reserved[4];
	uint16_t checksum;     // Checksum
} status_t;
#define STATUS_T_LEN (sizeof(uint8_t) * 10 + sizeof(uint16_t) + sizeof(int16_t) * 7 + sizeof(uint32_t) * 2 + sizeof(int32_t) * 2 + sizeof(int64_t))

#define MOVE_STATE_MOVING        0x01  // This flag indicates that controller is trying to move the motor
#define MOVE_STATE_TARGET_SPEED  0x02  // Target speed is reached, if flag set
#define MOVE_STATE_ANTIPLAY      0x04  // Motor is playing compensation, if flag set

#define MVCMD_NAME_BITS          0x3F  // Move command bit mask
#define MVCMD_UKNWN              0x00  // Unknown command
#define MVCMD_MOVE               0x01  // Command move
#define MVCMD_MOVR               0x02  // Command movr
#define MVCMD_LEFT               0x03  // Command left
#define MVCMD_RIGHT              0x04  // Command rigt
#define MVCMD_STOP               0x05  // Command stop
#define MVCMD_HOME               0x06  // Command home
#define MVCMD_LOFT               0x07  // Command loft
#define MVCMD_SSTP               0x08  // Command soft stop
#define MVCMD_ERROR              0x40  // Finish state (1 - move command have finished with an error, 0 - move command have finished correctly). This flags is actual when MVCMD_RUNNING signals movement finish
#define MVCMD_RUNNING            0x80  // Move command state (0 - move command have finished, 1 - move command is being executed)


#define PWR_STATE_UNKNOWN        0x00  // Unknown state, should never happen
#define PWR_STATE_OFF            0x01  // Motor windings are disconnected from the driver
#define PWR_STATE_NORM           0x03  // Motor windings are powered by nominal current
#define PWR_STATE_REDUCT         0x04  // Motor windings are powered by reduced current to lower power consumption
#define PWR_STATE_MAX            0x05  // Motor windings are powered by maximum current driver can provide at this voltage

#define ENC_STATE_ABSENT         0x00  // Encoder is absent
#define ENC_STATE_UNKNOWN        0x01  // Encoder state is unknown
#define ENC_STATE_MALFUNC        0x02  // Encoder is connected and malfunctioning
#define ENC_STATE_REVERS         0x03  // Encoder is connected and operational but counts in other direction
#define ENC_STATE_OK             0x04  // Encoder is connected and working properly

#define WIND_A_STATE_ABSENT      0x00  // Winding A is disconnected
#define WIND_A_STATE_UNKNOWN     0x01  // Winding A state is unknown
#define WIND_A_STATE_MALFUNC     0x02  // Winding A is short-circuited
#define WIND_A_STATE_OK          0x03  // Winding A is connected and working properly
#define WIND_B_STATE_ABSENT      0x00  // Winding B is disconnected
#define WIND_B_STATE_UNKNOWN     0x10  // Winding B state is unknown
#define WIND_B_STATE_MALFUNC     0x20  // Winding B is short-circuited
#define WIND_B_STATE_OK          0x30  // Winding B is connected and working properly

#define STATE_CONTR                    0x0003F  // Flags of controller states
#define STATE_ERRC                     0x00001  // Command error encountered
#define STATE_ERRD                     0x00002  // Data integrity error encountered
#define STATE_ERRV                     0x00004  // Value error encountered
#define STATE_EEPROM_CONNECTED         0x00010  // EEPROM with settings is connected
#define STATE_SECUR                    0x3FFC0  // Flags of security
#define STATE_ALARM                    0x00040  // Controller is in alarm state indicating that something dangerous had happened. Most commands are ignored in this state. To reset the flag a STOP command must be issued
#define STATE_CTP_ERROR                0x00080  // Control position error(is only used with stepper motor)
#define STATE_POWER_OVERHEAT           0x00100  // Power driver overheat
#define STATE_CONTROLLER_OVERHEAT      0x00200  // Controller overheat
#define STATE_OVERLOAD_POWER_VOLTAGE   0x00400  // Power voltage exceeds safe limit
#define STATE_OVERLOAD_POWER_CURRENT   0x00800  // Power current exceeds safe limit
#define STATE_OVERLOAD_USB_VOLTAGE     0x01000  // USB voltage exceeds safe limit
#define STATE_LOW_USB_VOLTAGE          0x02000  // USB voltage is insufficient for normal operation
#define STATE_OVERLOAD_USB_CURRENT     0x04000  // USB current exceeds safe limit
#define STATE_BORDERS_SWAP_MISSET      0x08000  // Engine stuck at the wrong edge
#define STATE_LOW_POWER_VOLTAGE        0x10000  // Power voltage is lower than Low Voltage Protection limit)
#define STATE_H_BRIDGE_FAULT           0x20000  // Signal from the driver that fault happened)

#define STATE_DIG_SIGNAL         0xFFFF  // Flags of digital signals
#define STATE_RIGHT_EDGE         0x0001  // Engine stuck at the right edge
#define STATE_LEFT_EDGE          0x0002  // Engine stuck at the left edge
#define STATE_BUTTON_RIGHT       0x0004  // Button "right" state (1 if pressed)
#define STATE_BUTTON_LEFT        0x0008  // Button "left" state (1 if pressed)
#define STATE_GPIO_PINOUT        0x0010  // External GPIO works as Out, if flag set; otherwise works as In
#define STATE_GPIO_LEVEL         0x0020  // State of external GPIO pin
#define STATE_HALL_A             0x0040  // State of Hall_a pin
#define STATE_HALL_B             0x0080  // State of Hall_b pin
#define STATE_HALL_C             0x0100  // State of Hall_c pin
#define STATE_BRAKE              0x0200  // State of Brake pin
#define STATE_REV_SENSOR         0x0400  // State of Revolution sensor pin
#define STATE_SYNC_INPUT         0x0800  // State of Sync input pin
#define STATE_SYNC_OUTPUT        0x1000  // State of Sync output pin
#define STATE_ENC_A              0x2000  // State of encoder A pin
#define STATE_ENC_B              0x4000  // State of encoder B pin

typedef struct _move_settings_t {
	uint32_t speed;           // Target speed (for stepper motor: steps/s, for DC: rpm). Range: 0..1000000.
	uint8_t uspeed;           // Target speed in 1/256 microsteps/s. Used with stepper motor only. Range: 0..255.
	uint16_t accel;           // Motor shaft acceleration, steps/s^2(stepper motor) or RPM/s(DC). Range: 0..65535.
	uint16_t decel;           // Motor shaft deceleration, steps/s^2(stepper motor) or RPM/s(DC). Range: 0..65535.
	uint32_t antiplay_speed;  // Speed in antiplay mode, full steps/s(stepper motor) or RPM. Range: 0..1000000.
	uint8_t antiplay_uspeed;  // Speed in antiplay mode, 1/256 microsteps/s. Used with stepper motor only. Range: 0..255.
	uint8_t reserved[10];
	uint16_t checksum;        // Checksum
} move_settings_t;
#define MOVE_SETTINGS_T_LEN (sizeof(uint8_t) * 12 + sizeof(uint16_t) * 3 + sizeof(uint32_t) * 2)


typedef struct _feedback_settings_t {
	uint16_t ips;          // The number of measured counts per revolution encoder
	uint8_t fb_type;       // Feedback type
	uint8_t flags;         // Feedback flags
	uint16_t hall_spr;     // The number of hall steps per revolution.
	int8_t hall_shift;     // Phase shift between output signal on BLDC engine and hall sensor input(0 - when only active the Hall sensor, the output state is a positive voltage on the winding A and a negative voltage on the winding B).
	uint8_t reserved[5];
	uint16_t checksum;     // Checksum
} feedback_settings_t;
#define FEEDBACK_SETTINGS_T_LEN (sizeof(uint8_t) * 7 + sizeof(int8_t) + sizeof(uint16_t) * 3)

#define FEEDBACK_ENCODER      0x01 // Feedback by encoder
#define FEEDBACK_ENCODERHALL  0x03 // Feedback by Hall detector
#define FEEDBACK_EMF          0x04 // Feedback by EMF
#define FEEDBACK_NONE         0x05 // Feedback is absent

#define FEEDBACK_ENC_REVERSE  0x01 // Reverse count of encoder
#define FEEDBACK_HALL_REVERSE 0x02 // Reverce count position on the Hall sensor


typedef struct _engine_settings_t {
	uint16_t nom_voltage;       // Rated voltage. Controller will keep the voltage drop on motor below this value if ENGINE_LIMIT_VOLT flag is set(Used with DC only). Range: 1..65535
	uint16_t nom_current;       // Rated current. Controller will keep current consumed by motor below this value if ENGINE_LIMIT_CURR flag is set. Range: 1..65535
	uint32_t nom_speed;         // Nominal speed (in whole steps / s or rpm for DC and stepper motor as a master encoder). Controller will keep motor shaft RPM below this value if ENGINE_LIMIT_RPM flag is set. Range: 1..1000000.
	uint8_t nom_uspeed;         // The fractional part of a nominal speed in microsteps (is only used with stepper motor). Range: 0..255
	uint16_t flags;             // Set of flags specify motor shaft movement algorithm and list of limitations
	int16_t antiplay;           // Number of pulses or steps for backlash (play) compensation procedure. Used if ENGINE_ANTIPLAY flag is set. Range: -32768..32767
	uint8_t microstep_mode;     // Settings of microstep mode (Used with stepper motor only)
	uint16_t steps_per_rev;     //
	uint8_t reserved[12];
	uint16_t checksum;          // Checksum
} engine_settings_t;
#define ENGINE_SETTINGS_T_LEN (sizeof(uint8_t) * 14 + sizeof(uint16_t) * 5 + sizeof(int16_t) + sizeof(uint32_t))

#define ENGINE_REVERSE     0x01  // Reverse flag. It determines motor shaft rotation direction that corresponds to feedback counts increasing. If not set (default), motor shaft rotation direction under positive voltage corresponds to feedback counts increasing and vice versa. Change it if you see that positive directions on motor and feedback are opposite
#define ENGINE_MAX_SPEED   0x04  // Max speed flag. If it is set, engine uses maxumum speed achievable with the present engine settings as nominal speed
#define ENGINE_ANTIPLAY    0x08  // Play compensation flag. If it set, engine makes backlash (play) compensation procedure and reach the predetermined position accurately on low speed
#define ENGINE_ACCEL_ON    0x10  // Acceleration enable flag. If it set, motion begins with acceleration and ends with deceleration
#define ENGINE_LIMIT_VOLT  0x20  // Maxumum motor voltage limit enable flag (is only used with DC motor)
#define ENGINE_LIMIT_CURR  0x40  // Maxumum motor current limit enable flag (is only used with DC motor)
#define ENGINE_LIMIT_RPM   0x80  // Maxumum motor speed limit enable flag

#define MICROSTEP_MODE_FULL      0x01  // Full step mode
#define MICROSTEP_MODE_FRAC_2    0x02  // 1/2 step mode
#define MICROSTEP_MODE_FRAC_4    0x03  // 1/4 step mode
#define MICROSTEP_MODE_FRAC_8    0x04  // 1/8 step mode
#define MICROSTEP_MODE_FRAC_16   0x05  // 1/16 step mode
#define MICROSTEP_MODE_FRAC_32   0x06  // 1/32 step mode
#define MICROSTEP_MODE_FRAC_64   0x07  // 1/64 step mode
#define MICROSTEP_MODE_FRAC_128  0x08  // 1/128 step mode
#define MICROSTEP_MODE_FRAC_256  0x09  // 1/256 step mode


typedef struct _home_settings_t {
	uint32_t fast_home;       // Speed used for first motion. Range: 0..1000000
	uint8_t ufast_home;       // Part of the speed for first motion, microsteps. Range: 0..255
	uint32_t slow_home;       // Speed used for second motion. Range: 0..1000000
	uint8_t uslow_home;       // Part of the speed for second motion, microsteps. Range: 0..255
	int32_t home_delta;       // Distance from break point. Range: -2147483647..2147483647
	int16_t uhome_delta;      // Part of the delta distance, microsteps. Range: -255..255
	uint16_t flags;           // Set of flags specify direction and stopping conditions
	uint8_t reserved[9];
	uint16_t checksum;        // Checksum
} home_settings_t;
#define HOME_SETTINGS_T_LEN  (sizeof(uint8_t) * 11 + sizeof(uint16_t) * 2 + sizeof(int16_t) + sizeof(uint32_t) * 2 + sizeof(int32_t))

#define HOME_DIR_FIRST         0x01  // Flag defines direction of 1st motion after execution of home command. Direction is right, if set; otherwise left
#define HOME_DIR_SECOND        0x02  // Flag defines direction of 2nd motion. Direction is right, if set; otherwise left
#define HOME_MV_SEC_EN         0x04  // Use the second phase of calibration to the home position, if set; otherwise the second phase is skipped
#define HOME_HALF_MV           0x08  // If the flag is set, the stop signals are ignored in start of second movement the first half-turn
#define HOME_STOP_FIRST_BITS   0x30  // Bits of the first stop selector
#define HOME_STOP_FIRST_REV    0x10  // First motion stops by revolution sensor
#define HOME_STOP_FIRST_SYN    0x20  // First motion stops by synchronization input
#define HOME_STOP_FIRST_LIM    0x30  // First motion stops by limit switch
#define HOME_STOP_SECOND_BITS  0xC0  // Bits of the second stop selector
#define HOME_STOP_SECOND_REV   0x40  // Second motion stops by revolution sensor
#define HOME_STOP_SECOND_SYN   0x80  // Second motion stops by synchronization input
#define HOME_STOP_SECOND_LIM   0xC0  // Second motion stops by limit switch



class XIMCProtocolHandler {
public:
	XIMCProtocolHandler() {};
	~XIMCProtocolHandler() {};

public:
	// Struct serialization
	int serialize(const position_t* obj, uint8_t *buffer, int length);
	int serialize(const sposition_t* obj, uint8_t *buffer, int length);
	int serialize(const move_settings_t* obj, uint8_t *buffer, int length);
	int serialize(const feedback_settings_t* obj, uint8_t *buffer, int length);
	int serialize(const engine_settings_t* obj, uint8_t *buffer, int length);
	int serialize(const home_settings_t* obj, uint8_t *buffer, int length);

	// Stuct un-serialization
	int unserialize(gposition_t* pos, const uint8_t *buffer, int length);
	int unserialize(status_t* status, const uint8_t *buffer, int length);
	int unserialize(move_settings_t* obj, const uint8_t *buffer, int length);
	int unserialize(feedback_settings_t* obj, const uint8_t *buffer, int length);
	int unserialize(engine_settings_t* obj, const uint8_t *buffer, int length);
	int unserialize(home_settings_t* obj, const uint8_t *buffer, int length);

protected:
	// Compute CRC16
	uint16_t crc16(const uint8_t *buffer, uint16_t length);
};

#endif
