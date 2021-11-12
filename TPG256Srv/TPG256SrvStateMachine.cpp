/*----- PROTECTED REGION ID(TPG256SrvStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        TPG256SrvStateMachine.cpp
//
// description : State machine file for the TPG256Srv class
//
// project :     Pfeiffer TPG256 Device Server
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

#include <TPG256Srv.h>

/*----- PROTECTED REGION END -----*/	//	TPG256Srv::TPG256SrvStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace TPG256Srv_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S1_name_allowed()
 *	Description : Execution allowed for S1_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S1_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S1_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S1_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S1_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S1_pressure_allowed()
 *	Description : Execution allowed for S1_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S1_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S1_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S1_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S1_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S1_status_allowed()
 *	Description : Execution allowed for S1_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S1_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S1_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S1_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S1_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S1_type_allowed()
 *	Description : Execution allowed for S1_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S1_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S1_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S1_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S1_typeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S2_name_allowed()
 *	Description : Execution allowed for S2_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S2_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S2_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S2_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S2_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S2_pressure_allowed()
 *	Description : Execution allowed for S2_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S2_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S2_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S2_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S2_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S2_status_allowed()
 *	Description : Execution allowed for S2_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S2_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S2_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S2_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S2_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S2_type_allowed()
 *	Description : Execution allowed for S2_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S2_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S2_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S2_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S2_typeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S3_name_allowed()
 *	Description : Execution allowed for S3_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S3_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S3_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S3_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S3_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S3_pressure_allowed()
 *	Description : Execution allowed for S3_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S3_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S3_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S3_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S3_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S3_status_allowed()
 *	Description : Execution allowed for S3_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S3_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S3_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S3_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S3_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S3_type_allowed()
 *	Description : Execution allowed for S3_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S3_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S3_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S3_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S3_typeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S4_name_allowed()
 *	Description : Execution allowed for S4_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S4_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S4_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S4_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S4_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S4_pressure_allowed()
 *	Description : Execution allowed for S4_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S4_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S4_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S4_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S4_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S4_status_allowed()
 *	Description : Execution allowed for S4_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S4_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S4_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S4_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S4_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S4_type_allowed()
 *	Description : Execution allowed for S4_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S4_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S4_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S4_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S4_typeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S5_name_allowed()
 *	Description : Execution allowed for S5_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S5_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S5_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S5_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S5_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S5_pressure_allowed()
 *	Description : Execution allowed for S5_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S5_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S5_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S5_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S5_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S5_status_allowed()
 *	Description : Execution allowed for S5_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S5_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S5_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S5_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S5_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S5_type_allowed()
 *	Description : Execution allowed for S5_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S5_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S5_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S5_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S5_typeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S6_name_allowed()
 *	Description : Execution allowed for S6_name attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S6_name_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S6_name attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S6_nameStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S6_nameStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S6_pressure_allowed()
 *	Description : Execution allowed for S6_pressure attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S6_pressure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S6_pressure attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S6_pressureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S6_pressureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S6_status_allowed()
 *	Description : Execution allowed for S6_status attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S6_status_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S6_status attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S6_statusStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S6_statusStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_S6_type_allowed()
 *	Description : Execution allowed for S6_type attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_S6_type_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for S6_type attribute in read access.
	/*----- PROTECTED REGION ID(TPG256Srv::S6_typeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::S6_typeStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_TurnOn_allowed()
 *	Description : Execution allowed for TurnOn attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_TurnOn_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for TurnOn command.
	/*----- PROTECTED REGION ID(TPG256Srv::TurnOnStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::TurnOnStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TPG256Srv::is_TurnOff_allowed()
 *	Description : Execution allowed for TurnOff attribute
 */
//--------------------------------------------------------
bool TPG256Srv::is_TurnOff_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for TurnOff command.
	/*----- PROTECTED REGION ID(TPG256Srv::TurnOffStateAllowed) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	TPG256Srv::TurnOffStateAllowed
	return true;
}


/*----- PROTECTED REGION ID(TPG256Srv::TPG256SrvStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	TPG256Srv::TPG256SrvStateAllowed.AdditionalMethods

}	//	End of namespace
