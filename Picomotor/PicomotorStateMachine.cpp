/*----- PROTECTED REGION ID(PicomotorStateMachine.cpp) ENABLED START -----*/
// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//=============================================================================
//
// file :        PicomotorStateMachine.cpp
//
// description : State machine file for the Picomotor class
//
// project :     Picomotor single-axis
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
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include <Picomotor.h>

/*----- PROTECTED REGION END -----*/	//	Picomotor::PicomotorStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace Picomotor_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_Velocity_allowed()
 *	Description : Execution allowed for Velocity attribute
 */
//--------------------------------------------------------
bool Picomotor::is_Velocity_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for Velocity attribute in Write access.
	/*----- PROTECTED REGION ID(Picomotor::VelocityStateAllowed_WRITE) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::VelocityStateAllowed_WRITE

	//	Not any excluded states for Velocity attribute in read access.
	/*----- PROTECTED REGION ID(Picomotor::VelocityStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::VelocityStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_Acceleration_allowed()
 *	Description : Execution allowed for Acceleration attribute
 */
//--------------------------------------------------------
bool Picomotor::is_Acceleration_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for Acceleration attribute in Write access.
	/*----- PROTECTED REGION ID(Picomotor::AccelerationStateAllowed_WRITE) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::AccelerationStateAllowed_WRITE

	//	Not any excluded states for Acceleration attribute in read access.
	/*----- PROTECTED REGION ID(Picomotor::AccelerationStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::AccelerationStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_Position_allowed()
 *	Description : Execution allowed for Position attribute
 */
//--------------------------------------------------------
bool Picomotor::is_Position_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for Position attribute in Write access.
	/*----- PROTECTED REGION ID(Picomotor::PositionStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	Picomotor::PositionStateAllowed_WRITE

	//	Not any excluded states for Position attribute in read access.
	/*----- PROTECTED REGION ID(Picomotor::PositionStateAllowed_READ) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::PositionStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_goHome_allowed()
 *	Description : Execution allowed for goHome attribute
 */
//--------------------------------------------------------
bool Picomotor::is_goHome_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for goHome command.
	/*----- PROTECTED REGION ID(Picomotor::goHomeStateAllowed) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::goHomeStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_setHome_allowed()
 *	Description : Execution allowed for setHome attribute
 */
//--------------------------------------------------------
bool Picomotor::is_setHome_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for setHome command.
	/*----- PROTECTED REGION ID(Picomotor::setHomeStateAllowed) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::setHomeStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : Picomotor::is_Stop_allowed()
 *	Description : Execution allowed for Stop attribute
 */
//--------------------------------------------------------
bool Picomotor::is_Stop_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for Stop command.
	/*----- PROTECTED REGION ID(Picomotor::StopStateAllowed) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	Picomotor::StopStateAllowed
	return true;
}


/*----- PROTECTED REGION ID(Picomotor::PicomotorStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	Picomotor::PicomotorStateAllowed.AdditionalMethods

}	//	End of namespace