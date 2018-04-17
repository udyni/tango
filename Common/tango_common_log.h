/*
 * TANGO common logger functions for exceptions
 *
 */

#ifndef TANGO_COMMON_LOG_H
#define TANGO_COMMON_LOG_H

#include <tango.h>
#include <sstream>


inline void error_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < e.errors.length(); i++) {
    msg << "[" << (i+1) << "] " << e.errors[i].origin << ": " << e.errors[i].desc << endl;
  }
  dev->get_logger()->error_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}


inline void warn_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < e.errors.length(); i++) {
    msg << "[" << (i+1) << "] " << e.errors[i].origin << ": " << e.errors[i].desc << endl;
  }
  dev->get_logger()->warn_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}


inline void info_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < e.errors.length(); i++) {
    msg << "[" << (i+1) << "] " << e.errors[i].origin << ": " << e.errors[i].desc << endl;
  }
  dev->get_logger()->info_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}


inline void debug_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < e.errors.length(); i++) {
    msg << "[" << (i+1) << "] " << e.errors[i].origin << ": " << e.errors[i].desc << endl;
  }
  dev->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}


#endif