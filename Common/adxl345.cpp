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


#include <adxl345.h>

#define ADXL345_MG2G_MULTIPLIER  0.004    // 4mg per lsb
#define STANDARD_GRAVITY         9.80665  // earth standard gravity

// Macro to measure elapsed time in ms
#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif

ADXL345::ADXL345(int bus, int address) :
  _fd(-1),
  _addr(address),
  _fifo(false)
{
  // Format adapter name
  char filename[20];
  ::snprintf(filename, 19, "/dev/i2c-%d", bus);

  // Open adapter
  _fd = ::open(filename, O_RDWR);
  if(_fd < 0) {
    throw ADXLException("Failed to open i2c adapter '%s' (Error: %s)", filename, strerror(errno));
  }

  // Configure device address
  if(ioctl(_fd, I2C_SLAVE, _addr) < 0) {
    throw ADXLException("Failed to configure device addess 0x%02X (Error: %s)", _addr, strerror(errno));
  }
#ifdef _DEBUG
  printf("Set slave address to 0x%02X\n", address);
#endif

  // Check adapter functionality
  unsigned long funcs = 0;
  if(ioctl(_fd, I2C_FUNCS, &funcs) < 0) {
      throw ADXLException("Failed to read adapter functionality (Error: %s)", _addr, strerror(errno));
  }

#ifdef _DEBUG
  // Print adapter functionality
  printf("Adapter %d on device file %s:\n", bus, filename);
  printf("Adapter functions flags: 0x%08lX\n", funcs);
  if(funcs & I2C_FUNC_I2C)
    printf("Adapter support I2C_FUNC_I2C\n");
  if(funcs & I2C_FUNC_10BIT_ADDR)
    printf("Adapter support I2C_FUNC_10BIT_ADDR\n");
  if(funcs & I2C_FUNC_PROTOCOL_MANGLING)
    printf("Adapter support I2C_FUNC_PROTOCOL_MANGLING\n");
  if(funcs & I2C_FUNC_SMBUS_PEC)
    printf("Adapter support I2C_FUNC_SMBUS_PEC\n");
  if(funcs & I2C_FUNC_NOSTART)
    printf("Adapter support I2C_FUNC_NOSTART\n");
  if(funcs & I2C_FUNC_SMBUS_BLOCK_PROC_CALL)
    printf("Adapter support I2C_FUNC_SMBUS_BLOCK_PROC_CALL\n");
  if(funcs & I2C_FUNC_SMBUS_QUICK)
    printf("Adapter support I2C_FUNC_SMBUS_QUICK\n");
  if(funcs & I2C_FUNC_SMBUS_READ_BYTE)
    printf("Adapter support I2C_FUNC_SMBUS_READ_BYTE\n");
  if(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)
    printf("Adapter support I2C_FUNC_SMBUS_WRITE_BYTE\n");
  if(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_READ_BYTE_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_WRITE_BYTE_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_READ_WORD_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_WRITE_WORD_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_PROC_CALL)
    printf("Adapter support I2C_FUNC_SMBUS_PROC_CALL\n");
  if(funcs & I2C_FUNC_SMBUS_READ_BLOCK_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_READ_BLOCK_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
    printf("Adapter support I2C_FUNC_SMBUS_WRITE_BLOCK_DATA\n");
  if(funcs & I2C_FUNC_SMBUS_READ_I2C_BLOCK)
    printf("Adapter support I2C_FUNC_SMBUS_READ_I2C_BLOCK\n");
  if(funcs & I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)
    printf("Adapter support I2C_FUNC_SMBUS_WRITE_I2C_BLOCK\n");
#endif

  // Check availability of I2C_FUNC_I2C
  if( !(funcs & I2C_FUNC_I2C) )
    throw ADXLException("I2C adapter does not support I2C transactions");

  // Initialize configuration of ADXL345 (equivalent to RESET)
  setRegister(ADXL_REG_THRESH_TAP, 0);
  setRegister(ADXL_REG_OFSX, 0);
  setRegister(ADXL_REG_OFSY, 0);
  setRegister(ADXL_REG_OFSZ, 0);
  setRegister(ADXL_REG_DUR, 0);
  setRegister(ADXL_REG_LATENT, 0);
  setRegister(ADXL_REG_WINDOW, 0);
  setRegister(ADXL_REG_THRESH_ACT, 0);
  setRegister(ADXL_REG_THRESH_INACT, 0);
  setRegister(ADXL_REG_TIME_INACT, 0);
  setRegister(ADXL_REG_ACT_INACT_CTL, 0);
  setRegister(ADXL_REG_THRESH_FF, 0);
  setRegister(ADXL_REG_TIME_FF, 0);
  setRegister(ADXL_REG_TAP_AXES, 0);
  setRegister(ADXL_REG_BW_RATE, 0x0A);
  setRegister(ADXL_REG_POWER_CTL, 0);
  setRegister(ADXL_REG_INT_ENABLE, 0);
  setRegister(ADXL_REG_INT_MAP, 0);
  setRegister(ADXL_REG_DATA_FORMAT, 0);
  setRegister(ADXL_REG_FIFO_CTL, 0);
}

// Destructor
ADXL345::~ADXL345() {
  // Close file
  if(_fd != -1)
    ::close(_fd);
}

// Copy constructor
ADXL345::ADXL345(const ADXL345 &obj) {
  _fd = ::dup(obj._fd);
  if(_fd < 0) {
    throw ADXLException("Failed to duplicate i2c adapter handle (Error: %s)", strerror(errno));
  }
  _addr = obj._addr;
}

// Assignment operator
ADXL345& ADXL345::operator= (const ADXL345 &obj) {
  if(_fd != -1) {
    ::close(_fd);
    _fd = -1;
  }
  _fd = ::dup(obj._fd);
  if(_fd < 0) {
    throw ADXLException("Failed to duplicate i2c adapter handle (Error: %s)", strerror(errno));
  }
  _addr = obj._addr;
  return *this;
}

// Write register value
void ADXL345::setRegister(uint8_t reg, uint8_t value) {
  // Format I2C message
  struct i2c_msg messages[1];
  uint8_t outbuf[2];
  messages[0].addr = _addr;
  messages[0].flags = 0;
  messages[0].len = sizeof(outbuf);
  messages[0].buf = outbuf;
  outbuf[0] = reg;
  outbuf[1] = value;

  // Send message
  struct i2c_rdwr_ioctl_data packets;
  packets.msgs  = messages;
  packets.nmsgs = 1;
  if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
    throw ADXLException("Failed to set device register 0x%02X (Error: %s)", reg, strerror(errno));
  }
}

// Read register value
uint8_t ADXL345::getRegister(uint8_t reg) {
  // Format I2C message
  struct i2c_msg messages[2];
  uint8_t outbuf, inbuf;
  messages[0].addr = _addr;
  messages[0].flags = 0;
  messages[0].len = sizeof(outbuf);
  messages[0].buf = &outbuf;
  outbuf = reg;
  messages[1].addr = _addr;
  messages[1].flags = I2C_M_RD;
  messages[1].len = sizeof(inbuf);
  messages[1].buf = &inbuf;

  // Send message
  struct i2c_rdwr_ioctl_data packets;
  packets.msgs  = messages;
  packets.nmsgs = 2;
  if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
    throw ADXLException("Failed to get device register 0x%02X (Error: %s)", reg, strerror(errno));
  }
  return inbuf;
}

// Dump registers
uint8_t ADXL345::dumpRegisters(uint8_t reg_start, uint8_t numreg, uint8_t* values, uint8_t length) {
  // Check the number of registers to read
  if(numreg == 0)
    return 0;

  if(values == NULL)
    throw ADXLException("No buffer to store results.");

  // Clear output values register
  memset(values, 0, length);

  if(numreg > length)
    numreg = length;

  // Format messages for sequential read
  struct i2c_msg messages[2];
  uint8_t outbuf;
  messages[0].addr = _addr;
  messages[0].flags = 0;
  messages[0].len = sizeof(outbuf);
  messages[0].buf = &outbuf;
  outbuf = reg_start;
  messages[1].addr = _addr;
  messages[1].flags = I2C_M_RD;
  messages[1].len = numreg;
  messages[1].buf = values;

  // Send message
  struct i2c_rdwr_ioctl_data packets;
  packets.msgs  = messages;
  packets.nmsgs = 2;
  if(ioctl(_fd, I2C_RDWR, &packets) < 0) {
    throw ADXLException("Failed to get dump device registers starting from 0x%02X for %d registers (Error: %s)", reg_start, int(numreg), strerror(errno));
  }
  return numreg;
}

void ADXL345::Start() {
  uint8_t reg = getRegister(ADXL_REG_POWER_CTL);
  if(!(reg & 0x08)) {
    reg |= 0x08;
    setRegister(ADXL_REG_POWER_CTL, reg);
  }
}

void ADXL345::Stop() {
  uint8_t reg = getRegister(ADXL_REG_POWER_CTL);
  if(reg & 0x08) {
    reg &= !(0x08);
    setRegister(ADXL_REG_POWER_CTL, reg);
  }
}

void ADXL345::setDataRate(uint8_t rate) {
  uint8_t reg = getRegister(ADXL_REG_BW_RATE);
  reg = (reg & 0xF0) | (rate & 0x0F);
  setRegister(ADXL_REG_BW_RATE, reg);
}

uint8_t ADXL345::getDataRate() {
  uint8_t reg = getRegister(ADXL_REG_BW_RATE);
  return (reg & 0x0F);
}

void ADXL345::setRange(uint8_t range) {
  uint8_t reg = getRegister(ADXL_REG_DATA_FORMAT);
  reg = (reg & (!0x3)) | (range & 0x03);
  setRegister(ADXL_REG_DATA_FORMAT, reg);
}

uint8_t ADXL345::getRange() {
  uint8_t reg = getRegister(ADXL_REG_DATA_FORMAT);
  return (reg & 0x03);
}

void ADXL345::getAcceleration(float& x, float& y, float& z, int* fifo) {
  // Read acceleration data
  uint8_t acc[8];
  if(_fifo) {
    dumpRegisters(ADXL_REG_DATAX0, 8, acc, 8);
  } else {
    dumpRegisters(ADXL_REG_DATAX0, 6, acc, 8);
  }
  x = double(acc[0] | (int16_t(acc[1]) << 8)) * ADXL345_MG2G_MULTIPLIER; // * STANDARD_GRAVITY;
  y = double(acc[2] | (int16_t(acc[3]) << 8)) * ADXL345_MG2G_MULTIPLIER; // * STANDARD_GRAVITY;
  z = double(acc[4] | (int16_t(acc[5]) << 8)) * ADXL345_MG2G_MULTIPLIER; // * STANDARD_GRAVITY;
  if(fifo != NULL) {
    if(_fifo) {
      *fifo = int(acc[7] & 0x3F);
    } else {
      *fifo = 0;
    }
  }
}

void ADXL345::setFifoMode(uint8_t mode) {
    uint8_t reg = getRegister(ADXL_REG_FIFO_CTL);
    reg &= 0x3F;
    reg |= (mode & 0xC0);
    if(mode > 0) {
        _fifo = true;
    } else {
        _fifo = false;
    }
    setRegister(ADXL_REG_FIFO_CTL, reg);
}

uint8_t ADXL345::getFifoMode() {
    uint8_t reg = getRegister(ADXL_REG_FIFO_CTL);
    return reg & 0xC0;
}

void ADXL345::setWatermark(uint8_t watermark) {
    uint8_t reg = getRegister(ADXL_REG_FIFO_CTL);
    reg &= 0xE0;
    reg |= (watermark & 0x1F);
    setRegister(ADXL_REG_FIFO_CTL, reg);
}

uint8_t ADXL345::getWatermark() {
    uint8_t reg = getRegister(ADXL_REG_FIFO_CTL);
    return reg & 0x1F;
}

void ADXL345::waitForFifo(uint8_t level, int32_t polling, int32_t timeout) {
  // Sleep for 500 us between che
  struct timespec slp;
  slp.tv_sec = polling / 1000000;
  slp.tv_nsec = (polling % 1000000) * 1000;

  // Structs to check for timeout
  struct timeval s, e;
  if(timeout > 0)
    gettimeofday(&s, NULL);

  while(true) {
    nanosleep(&slp, NULL);
    uint8_t reg = getRegister(ADXL_REG_FIFO_STATUS);
    if(int(reg & 0x3F) > level) {
      break;
    }
    if(timeout > 0) {
      gettimeofday(&e, NULL);
      int elapsed = ELAPSED_TIME_MS(s, e);
      if(elapsed > timeout) {
        throw ADXLException("Timed out checking FIFO level");
      }
    }
  }
}
