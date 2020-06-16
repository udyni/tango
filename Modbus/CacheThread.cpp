// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
//+*********************************************************************
//
// File:	CacheThread.cpp
//
// Project:	Device Servers in C++
//
// Description:	Code for implementing the Modbus data cache thread
//
// Author(s);	Emmanuel Taurel
//
// Original:	March 2008
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2009/02/25 13:58:40  buteau
// - files moved to src subdirectory
//
// Revision 1.5  2008/12/02 15:31:52  taurel
// - Before getting data in the cache, check that the thread is started
//
// Revision 1.4  2008/11/10 10:14:55  taurel
// - gettimeofday() is not defined in Windows!
//
// Revision 1.3  2008/07/01 10:53:25  taurel
// - The call to get date is now inside the loop
//
// Revision 1.2  2008/06/06 13:51:13  taurel
// - Better management of error when using the cache thread
// - Fix open file leak in case of TCP protocol and reconnection to the
// modbus device
//
// Revision 1.1  2008/03/17 14:37:34  taurel
// - Add a data cache for the ReadHoldingRegisters, ReadMultipleCoilsStatus, ReadInputRegisters and ReadInputStatus commands
// - Fix some bugs related to closing the sockets in TCP mode
// - The Address property is used for the modbus ID sent at the frame
// beginning
//
//
//
// Copyright (c) 2001 by European Synchrotron Radiation Facility,
//                       Grenoble, France
//
//
//
//-*********************************************************************

#include <Modbus.h>
#include <CacheThread.h>

namespace Modbus_ns
{

//+======================================================================
// Method:    CacheThread::run_undetached()
//
// Description:	create the instance of the thread used to fill up the
//				cahce
//
// Arg(s) In: - cdb : list of data to be cached
//			  - c_mut : mutex used to protect the thread command area
//			  - cmd : Pointer to the command area
//			  - s_time : Thread loop sleeping time
//			  - d_name : Modbus device name
//
//-=====================================================================

CacheThread::CacheThread(vector<CacheDataBlock> &cdb,omni_mutex &c_mut,ThreadCmd *cmd,long s_time,Modbus *d):
data_blocks(cdb),cmd_mutex(c_mut),th_cmd(cmd),the_dev(d)
{
	sleeping_time_s = s_time / 1000;
	sleeping_time_ns = (s_time % 1000) * 1000000;
}


//+======================================================================
// Method:    CacheThread::run_undetached()
//
// Description:	create a ModbusCore object
//
// Arg(s) In:
//
// Arg(s) Out:
//-=====================================================================

void *CacheThread::run_undetached(void *ptr)
{
	bool exit = false;
	unsigned long nb_block = data_blocks.size();
	struct timeval when;

//
// Thread loop
//

	try
	{
		while (exit == false)
		{
			//cout << "In cache Thread loop" << endl;

//
// Read thread command
//


			ThreadCmd the_cmd;
			{
				omni_mutex_lock sync(cmd_mutex);
				the_cmd = *th_cmd;
			}

			if (the_cmd == SUICIDE)
			{
				exit = true;
				continue;
			}

			for (unsigned long loop = 0;loop < nb_block;loop++)
			{
//				cout << "Cmd = " << data_blocks[loop].cmd_name << endl;
//				cout << "Adr = " << data_blocks[loop].in_args[0] << endl;
//				cout << "nb_data = " << data_blocks[loop].in_args[1] << endl;

#ifdef _TG_WINDOWS_
				struct _timeb now_win;
				_ftime(&now_win);
				when.tv_sec = (unsigned long)now_win.time;
				when.tv_usec = (long)now_win.millitm * 1000;
#else
				gettimeofday(&when,NULL);
#endif

//
// Read the data and copy them
// Do nit use command_inout on a DeviceProxy, because this will take
// the device monitor and therefore blocks other commands during the
// reading.
// Directly access the DeviceImpl method
// This means:
//  - Call the always_executed_hook() method
//  - Call the is_allowed() method. Not necessary in our case because these methods are empty
//  - Call the cmd method
//

				try
				{
					the_dev->always_executed_hook();
					if (data_blocks[loop].cmd_name == "readinputstatus")
					{
						Tango::DevVarCharArray *dvca = the_dev->read_input_status(&data_blocks[loop].in_args);
						{
							omni_mutex_lock sync(*(data_blocks[loop].data_block_mutex));
							::memcpy((void *)data_blocks[loop].char_data_cache_ptr,(void *)dvca->get_buffer(),(size_t)dvca->length());
							data_blocks[loop].err = false;
							data_blocks[loop].nb_sec = when.tv_sec;
						}
						delete dvca;
					}
					else if (data_blocks[loop].cmd_name == "readholdingregisters")
					{
						Tango::DevVarShortArray *dvsa = the_dev->read_holding_registers(&data_blocks[loop].in_args);
						{
							omni_mutex_lock sync(*(data_blocks[loop].data_block_mutex));
							::memcpy((void *)data_blocks[loop].short_data_cache_ptr,(void *)dvsa->get_buffer(),(size_t)dvsa->length() * 2);
							data_blocks[loop].err = false;
							data_blocks[loop].nb_sec = when.tv_sec;
						}
						delete dvsa;
					}
					else if (data_blocks[loop].cmd_name == "readmultiplecoilsstatus")
					{
						Tango::DevVarShortArray *dvsa = the_dev->read_multiple_coils_status(&data_blocks[loop].in_args);
						{
							omni_mutex_lock sync(*(data_blocks[loop].data_block_mutex));
							::memcpy((void *)data_blocks[loop].short_data_cache_ptr,(void *)dvsa->get_buffer(),(size_t)dvsa->length() * 2);
							data_blocks[loop].err = false;
							data_blocks[loop].nb_sec = when.tv_sec;
						}
						delete dvsa;
					}
					else
					{
						Tango::DevVarShortArray *dvsa = the_dev->read_input_registers(&data_blocks[loop].in_args);
						{
							omni_mutex_lock sync(*(data_blocks[loop].data_block_mutex));
							::memcpy((void *)data_blocks[loop].short_data_cache_ptr,(void *)dvsa->get_buffer(),(size_t)dvsa->length() * 2);
							data_blocks[loop].err = false;
							data_blocks[loop].nb_sec = when.tv_sec;
						}
						delete dvsa;
					}
				}
				catch (Tango::DevFailed &e)
				{
					Tango::Except::print_exception(e);
					{
						omni_mutex_lock sync(*(data_blocks[loop].data_block_mutex));
						data_blocks[loop].err = true;
						data_blocks[loop].errors = e.errors;
						data_blocks[loop].nb_sec = when.tv_sec;
					}
				}
			}

			omni_thread::sleep(sleeping_time_s,sleeping_time_ns);
		}
	}
	catch (omni_thread_fatal &otf)
	{
		cout << "omni_thread_fatal......." << endl;
	}

	return NULL;
}

} // End of namespace
