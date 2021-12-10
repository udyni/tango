/*----- PROTECTED REGION ID(VibrationMeter.h) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        VibrationMeter.h
//
// description : Include file for the VibrationMeter class
//
// project :     Vibration meter
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
// 2021 CNR-IFN
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef VibrationMeter_H
#define VibrationMeter_H

#include <tango.h>
#include <adxl355.h>
#include <omnithread.h>
#include <fftw3.h>
#include <cmath>

/*----- PROTECTED REGION END -----*/	//	VibrationMeter.h

/**
 *  VibrationMeter class description:
 *    Vibration meter based on ADXL345
 */

namespace VibrationMeter_ns
{
enum _SampleFrequencyEnum {
	_4000HZ,
	_2000HZ,
	_1000HZ,
	_500HZ,
	_250HZ,
	_125HZ,
	_62point5HZ,
	_31point25HZ,
	_15point625HZ,
	_7point8125HZ,
	_3point90625HZ,
} ;
typedef _SampleFrequencyEnum SampleFrequencyEnum;

/*----- PROTECTED REGION ID(VibrationMeter::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations
class AdxlDev;

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Additional Class Declarations

class VibrationMeter : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(VibrationMeter::Data Members) ENABLED START -----*/

//	Add your own data members

	AdxlDev* dev;

	Tango::DevShort *attr_SampleFrequency_value_read;

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Data Members

//	Device property data members
public:
	//	bus:	I2C bus number
	Tango::DevLong	bus;
	//	address:	I2C address
	Tango::DevLong	address;
	//	disable_fft:	Disable FFT calculation
	Tango::DevBoolean	disable_fft;

	bool	mandatoryNotDefined;

//	Attribute data members
public:
	Tango::DevULong	*attr_Samples_read;
	SampleFrequencyEnum	*attr_SampleFrequency_read;
	Tango::DevDouble	*attr_Period_read;
	Tango::DevDouble	*attr_FrequencyStep_read;
	Tango::DevDouble	*attr_X_spectrum_read;
	Tango::DevDouble	*attr_Y_spectrum_read;
	Tango::DevDouble	*attr_Z_spectrum_read;
	Tango::DevDouble	*attr_RawX_read;
	Tango::DevDouble	*attr_RawY_read;
	Tango::DevDouble	*attr_RawZ_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	VibrationMeter(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	VibrationMeter(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	VibrationMeter(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~VibrationMeter() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Read the device properties from database
	 */
	void get_device_property();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();

	/*
	 *	Check if mandatory property has been set
	 */
	 void check_mandatory_property(Tango::DbDatum &class_prop, Tango::DbDatum &dev_prop);

//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : VibrationMeter::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : VibrationMeter::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute Samples related methods
 *	Description: Number of samples
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
	virtual void read_Samples(Tango::Attribute &attr);
	virtual void write_Samples(Tango::WAttribute &attr);
	virtual bool is_Samples_allowed(Tango::AttReqType type);
/**
 *	Attribute SampleFrequency related methods
 *	Description: Sample Frequency
 *
 *	Data type:	Tango::DevEnum
 *	Attr type:	Scalar
 */
	virtual void read_SampleFrequency(Tango::Attribute &attr);
	virtual void write_SampleFrequency(Tango::WAttribute &attr);
	virtual bool is_SampleFrequency_allowed(Tango::AttReqType type);
/**
 *	Attribute Period related methods
 *	Description: Sample period in seconds
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_Period(Tango::Attribute &attr);
	virtual bool is_Period_allowed(Tango::AttReqType type);
/**
 *	Attribute FrequencyStep related methods
 *	Description: Frequecy step of the FFT
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_FrequencyStep(Tango::Attribute &attr);
	virtual bool is_FrequencyStep_allowed(Tango::AttReqType type);
/**
 *	Attribute X_spectrum related methods
 *	Description: Vibration spectrum on X axis
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_X_spectrum(Tango::Attribute &attr);
	virtual bool is_X_spectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute Y_spectrum related methods
 *	Description: Vibration spectrum on Y axis
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_Y_spectrum(Tango::Attribute &attr);
	virtual bool is_Y_spectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute Z_spectrum related methods
 *	Description: Vibration spectrum on Z axis
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_Z_spectrum(Tango::Attribute &attr);
	virtual bool is_Z_spectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute RawX related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_RawX(Tango::Attribute &attr);
	virtual bool is_RawX_allowed(Tango::AttReqType type);
/**
 *	Attribute RawY related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_RawY(Tango::Attribute &attr);
	virtual bool is_RawY_allowed(Tango::AttReqType type);
/**
 *	Attribute RawZ related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Spectrum max = 40960
 */
	virtual void read_RawZ(Tango::Attribute &attr);
	virtual bool is_RawZ_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : VibrationMeter::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:


	//--------------------------------------------------------
	/**
	 *	Method      : VibrationMeter::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(VibrationMeter::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Additional Method prototypes
};

/*----- PROTECTED REGION ID(VibrationMeter::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions
class AdxlDev : public omni_thread {
public:
	// Constructor
	AdxlDev(VibrationMeter* parent);

	// Destructor
	~AdxlDev();

	// Set rate
	void setDataRate(uint8_t rate);

	// Get data rate
	uint8_t getDataRate()const { return _samplerate; }

	// Get sample period
	double getPeriod()const { return _period; }

	// Set number of samples
	void setNSamples(uint32_t samples);

	// Get number of samples
	size_t getNSamples()const { return _nsamples; }

	// Frequecy step
	double getFrequecyStep() { return 1.0 / (_period * _nsamples); }

	// Terminate
	void terminate() { _terminate = true; }

protected:
	// Main loop function
	void* run_undetached(void *arg);

	// Convert sample rate to period
	double convertSampleRate(uint8_t rate);

	// Update period, number of samples and FFTW plan
	void updatePeriodSamples();

	// Compute shift and magnitude of FFT
	void computeShiftMagnitude(const fftw_complex* in, double *out, size_t length);

private:
	// Parent device
	VibrationMeter* _parent;

	// Access lock
	omni_mutex _lock;

	// ADXL device
	ADXL355* _dev;

	// Sample rate
	uint8_t _samplerate;

	// Sample period
	double _period;

	// Number of samples
	uint32_t _nsamples;

	// Buffer for X acceleration samples
	double* _x_buffer;

	// Buffer for Y acceleration samples
	double* _y_buffer;

	// Buffer for Z acceleration samples
	double* _z_buffer;

	// Sample index
	size_t _index;

	// Terminate flag
	bool _terminate;

	// FFTW plan
	fftw_plan _fft_p;

	// FFTW input array
	fftw_complex *_fft_in;

	// FFTW output array
	fftw_complex *_fft_out;
};

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Additional Classes Definitions

}	//	End of namespace

#endif   //	VibrationMeter_H