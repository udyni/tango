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


#define ADXL355_MG2G_MULTIPLIER  3.9E-6   // 3.9ug per lsb
#define STANDARD_GRAVITY         9.80665  // earth standard gravity

#include <adxl355.h>


ADXL355::ADXL355(int bus, int addr) :
  _fd(-1),
  _addr(addr)
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
    throw ADXLException("Failed to configure device address 0x%02X (Error: %s)", _addr, strerror(errno));
  }
#ifdef _DEBUG
  printf("Set slave address to 0x%02X\n", _addr);
#endif

  // Read
  uint8_t reg = getRegister(ADXL355_REG_DEVID_AD);
  if(reg != 0xAD) {
    throw ADXLException("Not an Analog Device chip (got 0x%02X instead of 0xAD", reg);
  }
#ifdef _DEBUG
  printf("Found Analog Device SPI chip (0x%02X)", reg);
#endif
  reg = getRegister(ADXL355_REG_DEVID_MST);
  if(reg != 0x1D) {
    throw ADXLException("Not an MEMS device (got 0x%02X instead of 0x1D", reg);
  }
#ifdef _DEBUG
  printf(", of MEMS type (0x%02X)", reg);
#endif
  reg = getRegister(ADXL355_REG_PARTID);
  if(reg != 0xED) {
    throw ADXLException("Not an ADXL355 device (got %o instead of 355)", reg);
  }
#ifdef _DEBUG
  printf(", model ADXL%o", reg);
#endif
  reg = getRegister(ADXL355_REG_REVID);
#ifdef _DEBUG
  printf(", revision ID %d\n", reg);
#endif

  // Soft reset by setting registers directly to avoid race conditions
  setRegister(ADXL355_REG_POWER_CTL, 0x01);
  setRegister(ADXL355_REG_OFFSETX_H, 0x00);
  setRegister(ADXL355_REG_OFFSETX_L, 0x00);
  setRegister(ADXL355_REG_OFFSETY_H, 0x00);
  setRegister(ADXL355_REG_OFFSETY_L, 0x00);
  setRegister(ADXL355_REG_OFFSETZ_H, 0x00);
  setRegister(ADXL355_REG_OFFSETZ_L, 0x00);
  setRegister(ADXL355_REG_ACT_EN, 0x00);
  setRegister(ADXL355_REG_ACT_TH_H, 0x00);
  setRegister(ADXL355_REG_ACT_TH_L, 0x00);
  setRegister(ADXL355_REG_ACT_COUNT, 0x01);
  setRegister(ADXL355_REG_FILTER, 0x00);
  setRegister(ADXL355_REG_FIFO_SAMPLES, 0x60);
  setRegister(ADXL355_REG_INT_MAP, 0x00);
  setRegister(ADXL355_REG_SYNC, 0x00);
  setRegister(ADXL355_REG_RANGE, 0x81);
  setRegister(ADXL355_REG_SELF_TEST, 0x00);

#if 0
  // Store shadow registers if needed
  uint8_t buffer[5];
  if(reg <= 0x01) {
    // Software reset can lead to a race condition. We should
    dumpRegisters(0x50, 5, buffer, 5);
  }

  struct timespec sl = {0, 100000000};
  size_t count = 0;
  while(true) {
    // Reset device
    setRegister(ADXL355_REG_RESET, 0x52);
    nanosleep(&sl, NULL);
    // Re-read and compare shadow registers if needed
    if(reg <= 0x01) {
      uint8_t new_buffer[5];
      dumpRegisters(0x50, 5, buffer, 5);
      for(size_t i = 0; i < 5; i++) {
        if(buffer[i] != new_buffer[i]) {
#ifdef _DEBUG
          printf("[WARN] failure in comparison of shadow registers. Resetting again.");
#endif
          if(count > 10) {
            throw ADXLException("Reset failed 10 times. Giving up. A power cycle is needed.");
          }
          count++;
          continue;
        }
      }
    }
    break;
  }
#endif
}

// Destructor
ADXL355::~ADXL355() {
  // Close file
  if(_fd != -1)
    ::close(_fd);
}

// Copy constructor
ADXL355::ADXL355(const ADXL355 &obj) {
  _fd = ::dup(obj._fd);
  if(_fd < 0) {
    throw ADXLException("Failed to duplicate I2C bus handle (Error: %s)", strerror(errno));
  }
  _addr = obj._addr;
}

// Assignment operator
ADXL355& ADXL355::operator= (const ADXL355 &obj) {
  if(_fd != -1) {
    ::close(_fd);
    _fd = -1;
  }
  _fd = ::dup(obj._fd);
  if(_fd < 0) {
    throw ADXLException("Failed to duplicate I2C bus handle (Error: %s)", strerror(errno));
  }
  _addr = obj._addr;
  return *this;
}


// Write register value
void ADXL355::setRegister(uint8_t reg, uint8_t value) {
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
uint8_t ADXL355::getRegister(uint8_t reg) {
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
uint8_t ADXL355::dumpRegisters(uint8_t reg_start, uint8_t numreg, uint8_t* values, uint8_t length) {
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

void ADXL355::Start() {
  uint8_t reg = getRegister(ADXL355_REG_POWER_CTL);
  if(reg & 0x01) {
    reg &= !(0x01);
    setRegister(ADXL355_REG_POWER_CTL, reg);
  }
}

void ADXL355::Stop() {
  uint8_t reg = getRegister(ADXL355_REG_POWER_CTL);
  if(!(reg & 0x01)) {
    reg |= 0x01;
    setRegister(ADXL355_REG_POWER_CTL, reg);
  }
}

void ADXL355::setDataRate(uint8_t rate) {
  uint8_t reg = getRegister(ADXL355_REG_FILTER);
  reg = (reg & 0xF0) | (rate & 0x0F);
  setRegister(ADXL355_REG_FILTER, reg);
}

uint8_t ADXL355::getDataRate() {
  uint8_t reg = getRegister(ADXL355_REG_FILTER);
  return (reg & 0x0F);
}

void ADXL355::setRange(uint8_t range) {
  uint8_t reg = getRegister(ADXL355_REG_RANGE);
  if(!(range & 0x03))
    range = 0x01;
  reg = (reg & (!0x03)) | (range & 0x03);
  setRegister(ADXL355_REG_RANGE, reg);
}

uint8_t ADXL355::getRange() {
  uint8_t reg = getRegister(ADXL355_REG_RANGE);
  return (reg & 0x03);
}

// Get FIFO samples
uint8_t ADXL355::getFifoSamples() {
  uint8_t reg = getRegister(ADXL355_REG_FIFO_ENTRIES);
  return reg & 0x7F;
}

// NOTE: this function should be called only if there are entries in the FIFO!
void ADXL355::getAcceleration(double& x, double& y, double& z) {
  uint8_t buffer[9];

  do {

    // Read first sample
    dumpRegisters(ADXL355_REG_FIFO_DATA, 3, buffer, 9);

    if(buffer[2] & 0x02) {
      // FIFO is empty!
      throw ADXLException("FIFO is empty. Wait for the FIFO to fill.");
    }

  } while(!(buffer[2] & 0x01));

  // Read 6 bytes to get the YZ acceleration
  dumpRegisters(ADXL355_REG_FIFO_DATA, 6, buffer+3, 6);

  int32_t ix = 0, iy = 0, iz = 0;
  ix = ( (((buffer[2] & 0xF0) << 8) | (buffer[1] << 16) | (buffer[0] << 24)) >> 12);
  iy = ( (((buffer[5] & 0xF0) << 8) | (buffer[4] << 16) | (buffer[3] << 24)) >> 12);
  iz = ( (((buffer[8] & 0xF0) << 8) | (buffer[7] << 16) | (buffer[6] << 24)) >> 12);

  x = double(ix) * ADXL355_MG2G_MULTIPLIER * STANDARD_GRAVITY;
  y = double(iy) * ADXL355_MG2G_MULTIPLIER * STANDARD_GRAVITY;
  z = double(iz) * ADXL355_MG2G_MULTIPLIER * STANDARD_GRAVITY;
}


double ADXL355::getTemperature() {
  uint8_t buffer[2];
  dumpRegisters(ADXL355_REG_TEMP2, 2, buffer, 2);

  uint16_t val = buffer[1] | ((buffer[0] & 0x0F) << 8);
#ifdef _DEBUG
  printf("Raw temperatuere: %d\n", val);
#endif

  return 25.0 - static_cast<double>(val - 1885) / 9.05;
}