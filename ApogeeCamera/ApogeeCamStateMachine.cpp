/*----- PROTECTED REGION ID(ApogeeCamStateMachine.cpp) ENABLED START -----*/
//=============================================================================
//
// file :        ApogeeCamStateMachine.cpp
//
// description : State machine file for the ApogeeCam class
//
// project :     Apogee CCD Camera
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

#include <ApogeeCam.h>

/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ApogeeCamStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace ApogeeCam_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_CoolingDrive_allowed()
 *	Description : Execution allowed for CoolingDrive attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_CoolingDrive_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for CoolingDrive attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingDriveStateAllowed_READ) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingDriveStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_CoolingEnable_allowed()
 *	Description : Execution allowed for CoolingEnable attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_CoolingEnable_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for CoolingEnable attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingEnableStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingEnableStateAllowed_WRITE

	//	Not any excluded states for CoolingEnable attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingEnableStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingEnableStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_CoolingSetpoint_allowed()
 *	Description : Execution allowed for CoolingSetpoint attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_CoolingSetpoint_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for CoolingSetpoint attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingSetpointStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingSetpointStateAllowed_WRITE

	//	Not any excluded states for CoolingSetpoint attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingSetpointStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingSetpointStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_CoolingTemperature_allowed()
 *	Description : Execution allowed for CoolingTemperature attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_CoolingTemperature_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for CoolingTemperature attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::CoolingTemperatureStateAllowed_READ) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::CoolingTemperatureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_EnableFastReadout_allowed()
 *	Description : Execution allowed for EnableFastReadout attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_EnableFastReadout_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for EnableFastReadout attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::EnableFastReadoutStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::EnableFastReadoutStateAllowed_WRITE

	//	Not any excluded states for EnableFastReadout attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::EnableFastReadoutStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::EnableFastReadoutStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_IntegrationTime_allowed()
 *	Description : Execution allowed for IntegrationTime attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_IntegrationTime_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for IntegrationTime attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::IntegrationTimeStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::IntegrationTimeStateAllowed_WRITE

	//	Not any excluded states for IntegrationTime attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::IntegrationTimeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::IntegrationTimeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_HBin_allowed()
 *	Description : Execution allowed for ROI_HBin attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_HBin_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_HBin attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HBinStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HBinStateAllowed_WRITE

	//	Not any excluded states for ROI_HBin attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HBinStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HBinStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_HSize_allowed()
 *	Description : Execution allowed for ROI_HSize attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_HSize_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_HSize attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HSizeStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HSizeStateAllowed_WRITE

	//	Not any excluded states for ROI_HSize attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HSizeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HSizeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_HStart_allowed()
 *	Description : Execution allowed for ROI_HStart attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_HStart_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_HStart attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HStartStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HStartStateAllowed_WRITE

	//	Not any excluded states for ROI_HStart attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_HStartStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_HStartStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_VBin_allowed()
 *	Description : Execution allowed for ROI_VBin attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_VBin_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_VBin attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VBinStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VBinStateAllowed_WRITE

	//	Not any excluded states for ROI_VBin attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VBinStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VBinStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_VSize_allowed()
 *	Description : Execution allowed for ROI_VSize attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_VSize_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_VSize attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VSizeStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VSizeStateAllowed_WRITE

	//	Not any excluded states for ROI_VSize attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VSizeStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VSizeStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_ROI_VStart_allowed()
 *	Description : Execution allowed for ROI_VStart attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_ROI_VStart_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for ROI_VStart attribute in Write access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VStartStateAllowed_WRITE) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VStartStateAllowed_WRITE

	//	Not any excluded states for ROI_VStart attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ROI_VStartStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ROI_VStartStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_Image_allowed()
 *	Description : Execution allowed for Image attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_Image_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for Image attribute in read access.
	/*----- PROTECTED REGION ID(ApogeeCam::ImageStateAllowed_READ) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ImageStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_StartAcquisition_allowed()
 *	Description : Execution allowed for StartAcquisition attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_StartAcquisition_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for StartAcquisition command.
	/*----- PROTECTED REGION ID(ApogeeCam::StartAcquisitionStateAllowed) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::StartAcquisitionStateAllowed
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : ApogeeCam::is_StopAcquisition_allowed()
 *	Description : Execution allowed for StopAcquisition attribute
 */
//--------------------------------------------------------
bool ApogeeCam::is_StopAcquisition_allowed(TANGO_UNUSED(const CORBA::Any &any))
{
	//	Not any excluded states for StopAcquisition command.
	/*----- PROTECTED REGION ID(ApogeeCam::StopAcquisitionStateAllowed) ENABLED START -----*/
	if(get_state() == Tango::FAULT)
		return false;
	
	/*----- PROTECTED REGION END -----*/	//	ApogeeCam::StopAcquisitionStateAllowed
	return true;
}


/*----- PROTECTED REGION ID(ApogeeCam::ApogeeCamStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	ApogeeCam::ApogeeCamStateAllowed.AdditionalMethods

}	//	End of namespace
