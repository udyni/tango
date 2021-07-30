// kate: replace-tabs on; indent-width 2; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 2;
/**
 * (c) 2021 - Michele Devetta (michele.devetta@cnr.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ADXL345_H
#define ADXL345_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <sys/time.h>


// ADXL345 default addresses
#define ADXL355_DEF_ADDRESS       0x1D    // MISO/ADSEL high
#define ADXL355_ALT_ADDRESS       0x53    // MISO/ADSEL low

// ADXL355 register map
#define ADXL355_REG_DEVID_AD      0x00    // Device ID (0xAD)
#define ADXL355_REG_DEVID_MST     0x01    // Device ID MST (0x1D)
#define ADXL355_REG_PARTID        0x02    // Part ID (0xED, 355 octal)
#define ADXL355_REG_REVID         0x03    // Revision ID
#define ADXL355_REG_STATUS        0x04    // Status
#define ADXL355_REG_FIFO_ENTRIES  0x05    // FIFO entries
#define ADXL355_REG_TEMP2         0x06    // Temperature register 2
#define ADXL355_REG_TEMP1         0x07    // Temperature register 1
#define ADXL355_REG_XDATA3        0x08    // X data register 3
#define ADXL355_REG_XDATA2        0x09    // X data register 2
#define ADXL355_REG_XDATA1        0x0A    // X data register 1
#define ADXL355_REG_YDATA3        0x0B    // Y data register 3
#define ADXL355_REG_YDATA2        0x0C    // Y data register 2
#define ADXL355_REG_YDATA1        0x0D    // Y data register 1
#define ADXL355_REG_ZDATA3        0x0E    // Z data register 3
#define ADXL355_REG_ZDATA2        0x0F    // Z data register 2
#define ADXL355_REG_ZDATA1        0x10    // Z data register 1
#define ADXL355_REG_FIFO_DATA     0x11    // FIFO data register
#define ADXL355_REG_OFFSETX_H     0x1E    // X offset high byte
#define ADXL355_REG_OFFSETX_L     0x1F    // X offset low byte
#define ADXL355_REG_OFFSETY_H     0x20    // Y offset high byte
#define ADXL355_REG_OFFSETY_L     0x21    // Y offset low byte
#define ADXL355_REG_OFFSETZ_H     0x22    // Z offset high byte
#define ADXL355_REG_OFFSETZ_L     0x23    // Z offset low byte
#define ADXL355_REG_ACT_EN        0x24
#define ADXL355_REG_ACT_TH_H      0x25
#define ADXL355_REG_ACT_TH_L      0x26
#define ADXL355_REG_ACT_COUNT     0x27
#define ADXL355_REG_FILTER        0x28
#define ADXL355_REG_FIFO_SAMPLES  0x29
#define ADXL355_REG_INT_MAP       0x2A
#define ADXL355_REG_SYNC          0x2B
#define ADXL355_REG_RANGE         0x2C
#define ADXL355_REG_POWER_CTL     0x2D
#define ADXL355_REG_SELF_TEST     0x2E
#define ADXL355_REG_RESET         0x2F

// Output data rates
#define ADXL355_ODR_4000          0x00   // 4000 Hz ODR and 1000 Hz BW
#define ADXL355_ODR_2000          0x01   // 2000 Hz ODR and 500 Hz BW
#define ADXL355_ODR_1000          0x02   // 1000 Hz ODR and 250 Hz BW
#define ADXL355_ODR_500           0x03   // 500 Hz ODR and 125 Hz BW
#define ADXL355_ODR_250           0x04   // 250 Hz ODR and 62.5 Hz BW
#define ADXL355_ODR_125           0x05   // 125 Hz ODR and 31.25 Hz BW
#define ADXL355_ODR_62P5          0x06   // 62.5 Hz ODR and 15.625 Hz BW
#define ADXL355_ODR_31P25         0x07   // 31.25 Hz ODR and 7.813 Hz BW
#define ADXL355_ODR_15P625        0x08   // 15.625 Hz ODR and 3.906 Hz BW
#define ADXL355_ODR_7P813         0x09   // 7.813 Hz ODR and 1.953 Hz BW
#define ADXL355_ODR_3P906         0x0A   // 3.906 Hz ODR and 0.977 Hz BW

// High-pass filter settings
#define ADXL355_HPF_BYPASS        0x00   // No filter
#define ADXL355_HPF_24P7          0x01   // 24.7 x 10^-4 x ODR
#define ADXL355_HPF_6P2           0x02   // 6.2084 x 10^-4 x ODR
#define ADXL355_HPF_1P5           0x03   // 1.5545 x 10^-4 x ODR
#define ADXL355_HPF_0P3           0x04   // 0.3862 x 10^-4 x ODR
#define ADXL355_HPF_0P1           0x05   // 0.0954 x 10^-4 x ODR
#define ADXL355_HPF_0P02          0x06   // 0.0238 x 10^-4 x ODR

// Ranges
#define ADXL355_RANGE_2G          0x01   // +/- 2G range
#define ADXL355_RANGE_4G          0x10   // +/- 4G range
#define ADXL355_RANGE_8G          0x11   // +/- 8G range

// @class ADXLException
#ifndef ADXL_EXCEPTION_CLASS
#define ADXL_EXCEPTION_CLASS
#define ADXL_ERROR_MSG_LEN 256

#include <stdarg.h>
#include <exception>

class ADXLException : public std::exception {
public:
  //! Constructor
  /*! Constructor of the exception. Can format an error message.
   *  @param f the format string (as in printf)
   *  @param ... variable parameters, corresponding to the format string.
   */
  ADXLException(const char* f, ...) {
    va_list ap;
    va_start(ap, f);
    vsnprintf(_msg, ADXL_ERROR_MSG_LEN, f, ap);
    va_end(ap);
  }
  ADXLException(const char* f, va_list ap) {
    vsnprintf(_msg, ADXL_ERROR_MSG_LEN, f, ap);
  }

  //! what(), return the error string
  virtual const char* what() const throw() {
    return _msg;
  }

private:
  char _msg[ADXL_ERROR_MSG_LEN];
};
#else
class ADXLException;
#endif



// @Class ADXL355
class ADXL355 {
public:
  // Constructor
  ADXL355(int bus, int addr);

  // Destructor
  ~ADXL355();

  // Copy constructor
  ADXL355(const ADXL355& obj);

  // Assignment operator
  ADXL355& operator= (const ADXL355& obj);

public:
  // Get/Set data rate
  uint8_t getDataRate();
  void setDataRate(uint8_t rate);

  // Get/Set measurement range
  uint8_t getRange();
  void setRange(uint8_t range);

  // Get FIFO samples
  uint8_t getFifoSamples();

  // Get acceleration
  // NOTE: this function should be called only if there are entries in the FIFO!
  void getAcceleration(double& x, double& y, double& z);

  // Get temperature
  double getTemperature();

  // Start measurement
  void Start();

  // Stop measurement
  void Stop();

public:
  // General purpose functions

  // Set device register
  void setRegister(uint8_t reg, uint8_t value);

  // Get device register
  uint8_t getRegister(uint8_t reg);

  // Dump a sequence of configuration registers
  uint8_t dumpRegisters(uint8_t reg_start, uint8_t numreg, uint8_t* values, uint8_t length);

private:
  int _fd;
  int _addr;
};






#endif
