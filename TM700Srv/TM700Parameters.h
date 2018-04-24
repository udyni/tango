#ifndef TM700_PARAMETERS_H
#define TM700_PARAMETERS_H

#include <stdint.h>
#include <map>
#include <string>

class TM700_param {
public:
	TM700_param() {clear(); };
	uint16_t dtype;
	bool b;
	float f;
	uint32_t i;
	string s;
	void clear() { dtype = 0; b = false; f = 0.0; i = 0; s = ""; }
};


// TM700 data types
#define TM700_DTYPE_BOOL    0
#define TM700_DTYPE_ULONG   1
#define TM700_DTYPE_FLOAT   2
#define TM700_DTYPE_STR     4
#define TM700_DTYPE_UINT    7
#define TM700_DTYPE_TEXT   11


// TM700 control parameters
#define TM700_PAR_HEATING      1  // Heating
#define TM700_PAR_STANDBY      2  // Standby
#define TM700_PAR_RUTCTRL      4  // Enable run-up time control
#define TM700_PAR_ERRACK       9  // Error acknowledgment
#define TM700_PAR_PUMPSTAT    10  // Pumping station on/off
#define TM700_PAR_ENVENT      12  // Enable venting
#define TM700_PAR_BRAKE       13  // Enable electric brake
#define TM700_PAR_SPSW        17  // Configure speed switch point
#define TM700_PAR_CFG_DO2     19  // Configure digital output 2
#define TM700_PAR_MOTOR       23  // Enable turbopump motor
#define TM700_PAR_CFG_D01     24  // Configure digital output 1
#define TM700_PAR_OP_BACKING  25  // Backing pump operation mode
#define TM700_PAR_SPST_MODE   26  // Rotation speed setting mode
#define TM700_PAR_GAS_MODE    27  // Gas mode
#define TM700_PAR_CFG_REMOTE  28  // Configuration remote
#define TM700_PAR_VENT_MODE   30  // Venting mode
#define TM700_PAR_CFG_ACC_A1  35  // Configuration of accessory A1
#define TM700_PAR_CFG_ACC_B1  36  // Configuration of accessory B1
#define TM700_PAR_CFG_ACC_A2  37  // Configuration of accessory A2
#define TM700_PAR_CFG_ACC_B2  38  // Configuration of accessory B2
#define TM700_PAR_CFG_REL_R1  45  // Configuration of relay R1
#define TM700_PAR_CFG_REL_R2  46  // Configuration of relay R2
#define TM700_PAR_CFG_REL_R3  47  // Configuration of relay R3
#define TM700_PAR_SEAL_GAS    50  // Sealing gas
#define TM700_PAR_CFG_AO1     55  // Configuration of output AO1
#define TM700_PAR_CFG_AI1     57  // Configuration of input AI1
#define TM700_PAR_CTRL_VIA    60  // Selection of control interface
#define TM700_PAR_CTRL_LOCK   61  // Locl control interface selection
#define TM700_PAR_CFG_DI1     62  // Configuration of input DI1
#define TM700_PAR_CFG_DI2     63  // Configuration of input DI2
#define TM700_PAR_CFG_DI3     64  // Configuration of input DI3

// TM700 status parameters
#define TM700_PAR_REMPRIO    300  // Remote priority
#define TM700_PAR_SPSW_ATT   302  // Rotation speed switch point attained
#define TM700_PAR_ERRCODE    303  // Error code
#define TM700_PAR_OT_ELEC    304  // Overtemperature of electronics
#define TM700_PAR_OT_PUMP    305  // Overtemperature of pump
#define TM700_PAR_SETSP_ATT  306  // Set rotation speed attained
#define TM700_PAR_PUMP_ACC   307  // Pump accelerates
#define TM700_PAR_SET_SP     308  // Set rotation speed
#define TM700_PAR_ROT_SP     309  // Actual rotation speed
#define TM700_PAR_CURR       310  // Drive current
#define TM700_PAR_OPH_PUMP   311  // Pump operating hours
#define TM700_PAR_FW_VER     312  // Firmware version
#define TM700_PAR_VOLT       313  // Drive voltage
#define TM700_PAR_OPH_ELEC   314  // Electronics operating hours
#define TM700_PAR_NOM_SP     315  // Nominal speed
#define TM700_PAR_POWER      316  // Drive power
#define TM700_PAR_PCYCLES    319  // Pump cycles
#define TM700_PAR_T_POWER    324  // Power stage temperature
#define TM700_PAR_T_ELEC     326  // Electronics temperature
#define TM700_PAR_BEAR_WEAR  329  // Bearing wear
#define TM700_PAR_T_PUMPB    330  // Pump bottom temperature
#define TM700_PAR_ACCEL      336  // Accleration/Deceleration
#define TM700_PAR_T_BEAR     342  // Bearings temperature
#define TM700_PAR_T_MOTOR    346  // Motor temperature
#define TM700_PAR_ELEC_NAME  349  // Drive unit name
#define TM700_PAR_HW_VER     354  // Hardware version
#define TM700_PAR_ROT_IMB    358  // Rotor imbalance
#define TM700_PAR_ERRHIST1   360  // Error history 1
#define TM700_PAR_ERRHIST2   361  // Error history 2
#define TM700_PAR_ERRHIST3   362  // Error history 3
#define TM700_PAR_ERRHIST4   363  // Error history 4
#define TM700_PAR_ERRHIST5   364  // Error history 5
#define TM700_PAR_ERRHIST6   365  // Error history 6
#define TM700_PAR_ERRHIST7   366  // Error history 7
#define TM700_PAR_ERRHIST8   367  // Error history 8
#define TM700_PAR_ERRHIST9   368  // Error history 9
#define TM700_PAR_ERRHIST10  369  // Error history 10
#define TM700_PAR_ROTSP_RPM  397  // Set rotation speed (rpm)
#define TM700_PAR_ACTSP_RPM  398  // Actual rotation speed (rpm)
#define TM700_PAR_NOMSP_RPM  399  // Nominal rotation speed (rpm)

// TM700 set value parameters
#define TM700_PAR_RUT_VAL    700  // Run-up time in minutes
#define TM700_PAR_SPSW1      701  // Rotation speed switch point 1
#define TM700_PAR_SPD_SVAL   707  // Rotation speed in rotation speed setting mode
#define TM700_PAR_PWR_SVAL   708  // Maximum power consumption
#define TM700_PAR_SWOFF      710  // Switch off threshold for pump in intermittent mode
#define TM700_PAR_SWON       711  // Switch on threshold for pump in intermittent mode
#define TM700_PAR_SB_SP      717  // Standby rotation speed
#define TM700_PAR_SPSW2      719  // Rotation speed switch point 2
#define TM700_PAR_VENT_SP    720  // Rotation speed to start venting
#define TM700_PAR_VENT_TIME  721  // Venting time
#define TM700_PAR_NOMSP_CON  777  // Nominal speed confirmation
#define TM700_PAR_RS485      797  // RS485 address



inline void TM700_generate_parameter_map(std::map<uint16_t, uint16_t> &map) {

	// TM700 control parameters
	map[TM700_PAR_HEATING]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_STANDBY]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_RUTCTRL]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_ERRACK]     = TM700_DTYPE_BOOL;
	map[TM700_PAR_PUMPSTAT]   = TM700_DTYPE_BOOL;
	map[TM700_PAR_ENVENT]     = TM700_DTYPE_BOOL;
	map[TM700_PAR_BRAKE]      = TM700_DTYPE_BOOL;
	map[TM700_PAR_SPSW]       = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_DO2]    = TM700_DTYPE_UINT;
	map[TM700_PAR_MOTOR]      = TM700_DTYPE_BOOL;
	map[TM700_PAR_CFG_D01]    = TM700_DTYPE_UINT;
	map[TM700_PAR_OP_BACKING] = TM700_DTYPE_UINT;
	map[TM700_PAR_SPST_MODE]  = TM700_DTYPE_UINT;
	map[TM700_PAR_GAS_MODE]   = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_REMOTE] = TM700_DTYPE_UINT;
	map[TM700_PAR_VENT_MODE]  = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_ACC_A1] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_ACC_B1] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_ACC_A2] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_ACC_B2] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_REL_R1] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_REL_R2] = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_REL_R3] = TM700_DTYPE_UINT;
	map[TM700_PAR_SEAL_GAS]   = TM700_DTYPE_BOOL;
	map[TM700_PAR_CFG_AO1]    = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_AI1]    = TM700_DTYPE_UINT;
	map[TM700_PAR_CTRL_VIA]   = TM700_DTYPE_UINT;
	map[TM700_PAR_CTRL_LOCK]  = TM700_DTYPE_BOOL;
	map[TM700_PAR_CFG_DI1]    = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_DI2]    = TM700_DTYPE_UINT;
	map[TM700_PAR_CFG_DI3]    = TM700_DTYPE_UINT;

	// TM700 status parameters
	map[TM700_PAR_REMPRIO]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_SPSW_ATT]   = TM700_DTYPE_BOOL;
	map[TM700_PAR_ERRCODE]    = TM700_DTYPE_STR;
	map[TM700_PAR_OT_ELEC]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_OT_PUMP]    = TM700_DTYPE_BOOL;
	map[TM700_PAR_SETSP_ATT]  = TM700_DTYPE_BOOL;
	map[TM700_PAR_PUMP_ACC]   = TM700_DTYPE_BOOL;
	map[TM700_PAR_SET_SP]     = TM700_DTYPE_ULONG;
	map[TM700_PAR_ROT_SP]     = TM700_DTYPE_ULONG;
	map[TM700_PAR_CURR]       = TM700_DTYPE_FLOAT;
	map[TM700_PAR_OPH_PUMP]   = TM700_DTYPE_ULONG;
	map[TM700_PAR_FW_VER]     = TM700_DTYPE_STR;
	map[TM700_PAR_VOLT]       = TM700_DTYPE_FLOAT;
	map[TM700_PAR_OPH_ELEC]   = TM700_DTYPE_ULONG;
	map[TM700_PAR_NOM_SP]     = TM700_DTYPE_ULONG;
	map[TM700_PAR_POWER]      = TM700_DTYPE_ULONG;
	map[TM700_PAR_PCYCLES]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_T_POWER]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_T_ELEC]     = TM700_DTYPE_ULONG;
	map[TM700_PAR_BEAR_WEAR]  = TM700_DTYPE_ULONG;
	map[TM700_PAR_T_PUMPB]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_ACCEL]      = TM700_DTYPE_ULONG;
	map[TM700_PAR_T_BEAR]     = TM700_DTYPE_ULONG;
	map[TM700_PAR_T_MOTOR]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_ELEC_NAME]  = TM700_DTYPE_STR;
	map[TM700_PAR_HW_VER]     = TM700_DTYPE_STR;
	map[TM700_PAR_ROT_IMB]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_ERRHIST1]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST2]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST3]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST4]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST5]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST6]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST7]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST8]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST9]   = TM700_DTYPE_STR;
	map[TM700_PAR_ERRHIST10]  = TM700_DTYPE_STR;
	map[TM700_PAR_ROTSP_RPM]  = TM700_DTYPE_ULONG;
	map[TM700_PAR_ACTSP_RPM]  = TM700_DTYPE_ULONG;
	map[TM700_PAR_NOMSP_RPM]  = TM700_DTYPE_ULONG;

	// TM700 set value parameters
	map[TM700_PAR_RUT_VAL]    = TM700_DTYPE_ULONG;
	map[TM700_PAR_SPSW1]      = TM700_DTYPE_ULONG;
	map[TM700_PAR_SPD_SVAL]   = TM700_DTYPE_FLOAT;
	map[TM700_PAR_PWR_SVAL]   = TM700_DTYPE_UINT;
	map[TM700_PAR_SWOFF]      = TM700_DTYPE_ULONG;
	map[TM700_PAR_SWON]       = TM700_DTYPE_ULONG;
	map[TM700_PAR_SB_SP]      = TM700_DTYPE_FLOAT;
	map[TM700_PAR_SPSW2]      = TM700_DTYPE_ULONG;
	map[TM700_PAR_VENT_SP]    = TM700_DTYPE_UINT;
	map[TM700_PAR_VENT_TIME]  = TM700_DTYPE_ULONG;
	map[TM700_PAR_NOMSP_CON]  = TM700_DTYPE_ULONG;
	map[TM700_PAR_RS485]      = TM700_DTYPE_ULONG;
}


// Convert error code to a human readable message
inline const char* get_error_message(uint16_t code) {
	switch(code) {
		case 1: // Err001
			return "No error";
		case 2:
			return "Overvoltage";
		case 6:
			return "Run-up time error";
		case 8:
			return "Connection electronic drive unit - pump faulty";
		case 10:
			return "Internal device fault";
		case 21:
			return "Electronic drive unit does not recognize pump";
		case 41:
			return "Excess current motor";
		case 43:
			return "Internal configuration fault";
		case 44:
			return "Excess temperature electronic";
		case 45:
			return "Excess temperature motor";
		case 46:
			return "Internal initialization fault";
		case 73:
			return "Overload axial bearing";
		case 74:
			return "Overload radial bearing";
		case 89:
			return "Rotor out of target area, stabilization impossible";
		case 91:
			return "Internal device fault";
		case 92:
			return "Unknown connection panel";
		case 93:
			return "Temperature analysis motor faulty";
		case 94:
			return "Temperature analysis electronic faulty";
		case 98:
			return "Internal communication fault";
		case 107:
			return "Collective fault power stage";
		case 108:
			return "Rotation speed measurement faulty";
		case 109:
			return "Firmware not confirmed";
		case 114:
			return "Temperature analysis power stage faulty";
		case 117:
			return "Excess temperature pump bottom part";
		case 118:
			return "Excess temperature power stage";
		case 119:
			return "Excess temperature bearing";
		case 777:
			return "Nominal rotation speed not confirmed";
		case 800:
			return "Excess current position sensors";
		case 802:
			return "Calibration of position sensors faulty";
		case 810:
			return "Data set missing in the pump";
		case 815:
			return "Excess current magnetic bearing output stage";
		case 890:
			return "Safety bearing stress > 100%";
		case 891:
			return "Rotor unbalance > 100%";
		default:
			return "Unknown error";
	}
}


// Check if error can be reset, given the rotation speed
inline bool error_can_be_reset(uint16_t code, uint16_t freq) {
	switch(code) {
		case 1:
		case 8:
		case 10:
		case 21:
		case 107:
		case 108:
		case 777:
			if(freq > 5)
				return false;
			else
				return true;
		default:
			return true;
	}
}


// Convert warning code to a human readable message
inline const char* get_warn_message(uint16_t code) {
	switch(code) {
		case 7:
			return "Low voltage / mains power failure";
		case 18:
			return "Remote priority conflict";
		case 45:
			return "High temperature motor";
		case 76:
			return "High temperature electronic";
		case 89:
			return "Rotor out of target area, stabilization was possible";
		case 97:
			return "Pump information invalid";
		case 98:
			return "Pump information incomplete";
		case 100:
			return "Rotation speed raised to minimum value";
		case 115:
			return "Temperature analysis pump bottm part faulty";
		case 116:
			return "Temperature analysis bearing faulty";
		case 117:
			return "High temperature pump bottom part";
		case 118:
			return "High temperature power stage";
		case 119:
			return "High temperature bearing";
		case 168:
			return "High deceleration";
		case 801:
			return "Brake electronics defective";
		case 806:
			return "Brake resistor defective";
		case 807:
			return "Calibration position sensors required";
		case 890:
			return "Safety bearing stress > 75%";
		case 891:
			return "Rotor unbalance > 75%";
		default:
			return "Unknown warning";
	}
}

#endif