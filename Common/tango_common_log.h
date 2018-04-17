/*
 * TANGO common logger functions for exceptions
 *
 */

#ifndef TANGO_COMMON_LOG_H
#define TANGO_COMMON_LOG_H

#include <tango.h>
#include <sstream>


inline void error_log_errorlist(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevErrorList &errors) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < errors.length(); i++) {
    msg << "[" << (i+1) << "] " << errors[i].origin << ": " << errors[i].desc << endl;
  }
  dev->get_logger()->error_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}

inline void error_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  error_log_errorlist(dev, std_msg, e.errors);
}


inline void warn_log_errorlist(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevErrorList &errors) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < errors.length(); i++) {
    msg << "[" << (i+1) << "] " << errors[i].origin << ": " << errors[i].desc << endl;
  }
  dev->get_logger()->warn_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}

inline void warn_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  warn_log_errorlist(dev, std_msg, e.errors);
}


inline void info_log_errorlist(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevErrorList &errors) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < errors.length(); i++) {
    msg << "[" << (i+1) << "] " << errors[i].origin << ": " << errors[i].desc << endl;
  }
  dev->get_logger()->info_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}

inline void info_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  info_log_errorlist(dev, std_msg, e.errors);
}


inline void debug_log_errorlist(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevErrorList &errors) {
  std::stringstream msg;
  msg << std_msg << endl;
  for(size_t i = 0; i < errors.length(); i++) {
    msg << "[" << (i+1) << "] " << errors[i].origin << ": " << errors[i].desc << endl;
  }
  dev->get_logger()->debug_stream() << log4tango::LogInitiator::_begin_log << msg.str();
}

inline void debug_log_exception(Tango::DeviceImpl *dev, const char* std_msg, Tango::DevFailed &e) {
  debug_log_errorlist(dev, std_msg, e.errors);
}


#endif