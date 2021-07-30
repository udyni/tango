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
#define ADXL345_DEF_ADDRESS       0x1D    // SDO/ALTADDR high
#define ADXL345_ALT_ADDRESS       0x53    // SDO/ALTADDR low

// ADXL345 Data rates
#define ADXL_RATE_0_10HZ           0
#define ADXL_RATE_0_20HZ           1
#define ADXL_RATE_0_39HZ           2
#define ADXL_RATE_0_78HZ           3
#define ADXL_RATE_1_56HZ           4
#define ADXL_RATE_3_13HZ           5
#define ADXL_RATE_6_25HZ           6
#define ADXL_RATE_12_05HZ          7
#define ADXL_RATE_25HZ             8
#define ADXL_RATE_50HZ             9
#define ADXL_RATE_100HZ           10
#define ADXL_RATE_200HZ           11
#define ADXL_RATE_400HZ           12
#define ADXL_RATE_800HZ           13
#define ADXL_RATE_1600HZ          14
#define ADXL_RATE_3200HZ          15

// ADXL345 measurement ranges
#define ADXL_RANGE_2G             0
#define ADXL_RANGE_4G             1
#define ADXL_RANGE_8G             2
#define ADXL_RANGE_16G            3

// ADXL345 register map
#define ADXL_REG_DEVID            0x00    // Device ID (0xE5)
#define ADXL_REG_THRESH_TAP       0x1D    // Tap threshold
#define ADXL_REG_OFSX             0x1E    // X-axis offset
#define ADXL_REG_OFSY             0x1F    // Y-axis offset
#define ADXL_REG_OFSZ             0x20    // Z-axis offset
#define ADXL_REG_DUR              0x21    // Tap duration
#define ADXL_REG_LATENT           0x22    // Tap latency
#define ADXL_REG_WINDOW           0x23    // Tap window
#define ADXL_REG_THRESH_ACT       0x24    // Activity threshold
#define ADXL_REG_THRESH_INACT     0x25    // Inactivity threshold
#define ADXL_REG_TIME_INACT       0x26    // Inactivity time
#define ADXL_REG_ACT_INACT_CTL    0x27    // Axis enable control for activity and inactivity detection
#define ADXL_REG_THRESH_FF        0x28    // Free-fall threshold
#define ADXL_REG_TIME_FF          0x29    // Free-fall time
#define ADXL_REG_TAP_AXES         0x2A    // Axis control for single tap/double tap
#define ADXL_REG_ACT_TAP_STATUS   0x2B    // Source of single tap/double tap
#define ADXL_REG_BW_RATE          0x2C    // Data rate and power mode control
#define ADXL_REG_POWER_CTL        0x2D    // Power-saving features control
#define ADXL_REG_INT_ENABLE       0x2E    // Interrupt enable control
#define ADXL_REG_INT_MAP          0x2F    // Interrupt mapping control
#define ADXL_REG_INT_SOURCE       0x30    // Source of interrupts
#define ADXL_REG_DATA_FORMAT      0x31    // Data format control
#define ADXL_REG_DATAX0           0x32    // X-Axis Data 0
#define ADXL_REG_DATAX1           0x33    // X-Axis Data 1
#define ADXL_REG_DATAY0           0x34    // Y-Axis Data 0
#define ADXL_REG_DATAY1           0x35    // Y-Axis Data 1
#define ADXL_REG_DATAZ0           0x36    // Z-Axis Data 0
#define ADXL_REG_DATAZ1           0x37    // Z-Axis Data 1
#define ADXL_REG_FIFO_CTL         0x38    // FIFO control
#define ADXL_REG_FIFO_STATUS      0x39    // FIFO status

// ADXL345 FIFO control
#define ADXL_FIFO_BYPASS          0x00    // FIFO is disabled
#define ADXL_FIFO_FIFO            0x40    // FIFO collects up to 32 samples then stops
#define ADXL_FIFO_STREAM          0x80    // FIFO holds the last 32 samples
#define ADXL_FIFO_TRIGGER         0xC0    // Trigger mode

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



// @Class ADXL345
class ADXL345 {
public:
  // Constructor
  ADXL345(int i2c_bus, int address);

  // Destructor
  ~ADXL345();

  // Copy constructor
  ADXL345(const ADXL345& obj);

  // Assignment operator
  ADXL345& operator= (const ADXL345& obj);

public:
  // Get/Set data rate
  uint8_t getDataRate();
  void setDataRate(uint8_t rate);

  // Get/Set measurement range
  uint8_t getRange();
  void setRange(uint8_t range);

  // Get acceleration
  void getAcceleration(float& x, float& y, float& z, int* fifo=NULL);

  // Start measurement
  void Start();

  // Stop measurement
  void Stop();

  // Set fifo mode
  void setFifoMode(uint8_t mode);

  // Get fifo mode
  uint8_t getFifoMode();

  // Set watermark
  void setWatermark(uint8_t watermark);

  // Get watermark
  uint8_t getWatermark();

  // Wait for FIFO level (polling is in us, timeout in ms)
  void waitForFifo(uint8_t level, int32_t polling, int32_t timeout=-1);

public:
  // General purpose functions

  // Set device register
  void setRegister(uint8_t reg, uint8_t value);

  // Get device register
  uint8_t getRegister(uint8_t reg);

  // Dump a sequence of configuration registers
  uint8_t dumpRegisters(uint8_t reg_start, uint8_t numreg, uint8_t* values, uint8_t length);

private:
  int _addr;
  int _fd;
  bool _fifo;
};


#endif
