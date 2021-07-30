/*----- PROTECTED REGION ID(VibrationMeterStateMachine.cpp) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        VibrationMeterStateMachine.cpp
//
// description : State machine file for the VibrationMeter class
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

#include <VibrationMeter.h>

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::VibrationMeterStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace VibrationMeter_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_Samples_allowed()
 *	Description : Execution allowed for Samples attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_Samples_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for Samples attribute in Write access.
	/*----- PROTECTED REGION ID(VibrationMeter::SamplesStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::SamplesStateAllowed_WRITE

	//	Not any excluded states for Samples attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::SamplesStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::SamplesStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_SampleFrequency_allowed()
 *	Description : Execution allowed for SampleFrequency attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_SampleFrequency_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for SampleFrequency attribute in Write access.
	/*----- PROTECTED REGION ID(VibrationMeter::SampleFrequencyStateAllowed_WRITE) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::SampleFrequencyStateAllowed_WRITE

	//	Not any excluded states for SampleFrequency attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::SampleFrequencyStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::SampleFrequencyStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_Period_allowed()
 *	Description : Execution allowed for Period attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_Period_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for Period attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::PeriodStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::PeriodStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_FrequencyStep_allowed()
 *	Description : Execution allowed for FrequencyStep attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_FrequencyStep_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for FrequencyStep attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::FrequencyStepStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::FrequencyStepStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_X_spectrum_allowed()
 *	Description : Execution allowed for X_spectrum attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_X_spectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for X_spectrum attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::X_spectrumStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::X_spectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_Y_spectrum_allowed()
 *	Description : Execution allowed for Y_spectrum attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_Y_spectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for Y_spectrum attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::Y_spectrumStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Y_spectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_Z_spectrum_allowed()
 *	Description : Execution allowed for Z_spectrum attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_Z_spectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for Z_spectrum attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::Z_spectrumStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::Z_spectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_RawX_allowed()
 *	Description : Execution allowed for RawX attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_RawX_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for RawX attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::RawXStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::RawXStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_RawY_allowed()
 *	Description : Execution allowed for RawY attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_RawY_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for RawY attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::RawYStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::RawYStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : VibrationMeter::is_RawZ_allowed()
 *	Description : Execution allowed for RawZ attribute
 */
//--------------------------------------------------------
bool VibrationMeter::is_RawZ_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for RawZ attribute in read access.
	/*----- PROTECTED REGION ID(VibrationMeter::RawZStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	VibrationMeter::RawZStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================


/*----- PROTECTED REGION ID(VibrationMeter::VibrationMeterStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	VibrationMeter::VibrationMeterStateAllowed.AdditionalMethods

}	//	End of namespace
