/*----- PROTECTED REGION ID(MercurySrvStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        MercurySrvStateMachine.cpp
//
// description : State machine file for the MercurySrv class
//
// project :     Mercury C863 Server
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
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include <MercurySrv.h>

/*----- PROTECTED REGION END -----*/	//	MercurySrv::MercurySrvStateMachine.cpp

//================================================================
//  States   |  Description
//================================================================
//  DISABLE  |  Device disabled. Configuration/controller mismatch.
//  INIT     |  Initialization in progress
//  FAULT    |  Communication failure
//  MOVING   |  Motion in progress.
//  STANDBY  |  Ready.


namespace MercurySrv_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Acceleration_allowed()
 *	Description : Execution allowed for Acceleration attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Acceleration_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Check access type.
	if ( type!=Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for WRITE 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::INIT ||
			get_state()==Tango::FAULT ||
			get_state()==Tango::MOVING)
		{
		/*----- PROTECTED REGION ID(MercurySrv::AccelerationStateAllowed_WRITE) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	MercurySrv::AccelerationStateAllowed_WRITE
			return false;
		}
		return true;
	}
	else

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::FAULT)
		{
		/*----- PROTECTED REGION ID(MercurySrv::AccelerationStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::AccelerationStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Velocity_allowed()
 *	Description : Execution allowed for Velocity attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Velocity_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Check access type.
	if ( type!=Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for WRITE 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::INIT ||
			get_state()==Tango::FAULT ||
			get_state()==Tango::MOVING)
		{
		/*----- PROTECTED REGION ID(MercurySrv::VelocityStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::VelocityStateAllowed_WRITE
			return false;
		}
		return true;
	}
	else

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::FAULT)
		{
		/*----- PROTECTED REGION ID(MercurySrv::VelocityStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::VelocityStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_RawPosition_allowed()
 *	Description : Execution allowed for RawPosition attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_RawPosition_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Check access type.
	if ( type!=Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for WRITE 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::INIT ||
			get_state()==Tango::FAULT ||
			get_state()==Tango::MOVING)
		{
		/*----- PROTECTED REGION ID(MercurySrv::RawPositionStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::RawPositionStateAllowed_WRITE
			return false;
		}
		return true;
	}
	else

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::FAULT)
		{
		/*----- PROTECTED REGION ID(MercurySrv::RawPositionStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::RawPositionStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Position_allowed()
 *	Description : Execution allowed for Position attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Position_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Check access type.
	if ( type!=Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for WRITE 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::INIT ||
			get_state()==Tango::FAULT ||
			get_state()==Tango::MOVING)
		{
		/*----- PROTECTED REGION ID(MercurySrv::PositionStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::PositionStateAllowed_WRITE
			return false;
		}
		return true;
	}
	else

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::FAULT)
		{
		/*----- PROTECTED REGION ID(MercurySrv::PositionStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::PositionStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Time_allowed()
 *	Description : Execution allowed for Time attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Time_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Check access type.
	if ( type!=Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for WRITE 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::INIT ||
			get_state()==Tango::FAULT ||
			get_state()==Tango::MOVING)
		{
		/*----- PROTECTED REGION ID(MercurySrv::TimeStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::TimeStateAllowed_WRITE
			return false;
		}
		return true;
	}
	else

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE ||
			get_state()==Tango::FAULT)
		{
		/*----- PROTECTED REGION ID(MercurySrv::TimeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::TimeStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_HomePosition_allowed()
 *	Description : Execution allowed for HomePosition attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_HomePosition_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE)
		{
		/*----- PROTECTED REGION ID(MercurySrv::HomePositionStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::HomePositionStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_StageName_allowed()
 *	Description : Execution allowed for StageName attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_StageName_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE)
		{
		/*----- PROTECTED REGION ID(MercurySrv::StageNameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::StageNameStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_LastError_allowed()
 *	Description : Execution allowed for LastError attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_LastError_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Check access type.
	if ( type==Tango::READ_REQ )
	{
		//	Compare device state with not allowed states for READ 
		if (get_state()==Tango::DISABLE)
		{
		/*----- PROTECTED REGION ID(MercurySrv::LastErrorStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::LastErrorStateAllowed_READ
			return false;
		}
		return true;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_MotionDone_allowed()
 *	Description : Execution allowed for MotionDone attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_MotionDone_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for MotionDone attribute in read access.
	/*----- PROTECTED REGION ID(MercurySrv::MotionDoneStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::MotionDoneStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Macro_allowed()
 *	Description : Execution allowed for Macro attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Macro_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for Macro attribute in Write access.
	/*----- PROTECTED REGION ID(MercurySrv::MacroStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::MacroStateAllowed_WRITE

	//	Not any excluded states for Macro attribute in read access.
	/*----- PROTECTED REGION ID(MercurySrv::MacroStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::MacroStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_setHome_allowed()
 *	Description : Execution allowed for setHome attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_setHome_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT ||
		get_state()==Tango::MOVING)
	{
	/*----- PROTECTED REGION ID(MercurySrv::setHomeStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::setHomeStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_goHome_allowed()
 *	Description : Execution allowed for goHome attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_goHome_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT ||
		get_state()==Tango::MOVING)
	{
	/*----- PROTECTED REGION ID(MercurySrv::goHomeStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::goHomeStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_timeStep_allowed()
 *	Description : Execution allowed for timeStep attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_timeStep_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT ||
		get_state()==Tango::MOVING)
	{
	/*----- PROTECTED REGION ID(MercurySrv::timeStepStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::timeStepStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_posStep_allowed()
 *	Description : Execution allowed for posStep attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_posStep_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT ||
		get_state()==Tango::MOVING)
	{
	/*----- PROTECTED REGION ID(MercurySrv::posStepStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::posStepStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Stop_allowed()
 *	Description : Execution allowed for Stop attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Stop_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(MercurySrv::StopStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::StopStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Abort_allowed()
 *	Description : Execution allowed for Abort attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Abort_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::FAULT)
	{
	/*----- PROTECTED REGION ID(MercurySrv::AbortStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::AbortStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_Reset_allowed()
 *	Description : Execution allowed for Reset attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_Reset_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE ||
		get_state()==Tango::INIT ||
		get_state()==Tango::MOVING)
	{
	/*----- PROTECTED REGION ID(MercurySrv::ResetStateAllowed) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	MercurySrv::ResetStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_StartMacro_allowed()
 *	Description : Execution allowed for StartMacro attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_StartMacro_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE)
	{
	/*----- PROTECTED REGION ID(MercurySrv::StartMacroStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::StartMacroStateAllowed
		return false;
	}
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : MercurySrv::is_StopMacro_allowed()
 *	Description : Execution allowed for StopMacro attribute
 */
//--------------------------------------------------------
bool MercurySrv::is_StopMacro_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Compare device state with not allowed states.
	if (get_state()==Tango::DISABLE)
	{
	/*----- PROTECTED REGION ID(MercurySrv::StopMacroStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	MercurySrv::StopMacroStateAllowed
		return false;
	}
	return true;
}


/*----- PROTECTED REGION ID(MercurySrv::MercurySrvStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	MercurySrv::MercurySrvStateAllowed.AdditionalMethods

}	//	End of namespace