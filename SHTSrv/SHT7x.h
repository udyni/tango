// kate: replace-tabs on; indent-width 2; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 2
/*
 * Sensirion SHT7* interface for Raspberry Pi with wiringPi
 */
#ifndef __SENSIRION_SHT7X_H
#define __SENSIRION_SHT7X_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <wiringPi.h>

                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1E   //000   1111    0

#ifndef DELAY_MULTIPLIER
#define DELAY_MULTIPLIER 1
#endif


class SensirionSHT {
public:
  // Constructor
  SensirionSHT(int sck, int sda) : _sck(sck), _sda(sda), _t(0), _h(0) {}

  // Initialize sensor interface
  void init();

  // Start a T and H measure
  int measure();

  // Get temperature in °C
  float getT() { return _temp; }

  // Get temperature compensated relative humidity
  float getH() { return _humi; }

  // Get dew point
  float getD() { return computeD(_temp, _humi); }

  // Compute dew point
  static float computeD(float t, float h);

private:
  // Send a start sequence
  void trStart();

  // Send one byte to sensor
  int sendByte(uint8_t b);

  // Receive one byte from sensor
  uint8_t recvByte(uint8_t ack);

  // Reset connection
  void connReset();

  // Measure value
  int measureValue(uint16_t& val, uint8_t cmd);

  // Compute CRC
  uint8_t computeCRC(uint8_t cmd, uint8_t hi, uint8_t lo);

  // Reverse a byte bitwise
  uint8_t reverse(uint8_t b);

  // Get temperature in °C
  float computeT() { return 0.01 * float(_t) - 40.1; }

  // Get temperature compensated relative humidity
  float computeH();

private:
  int _sck;
  int _sda;

  // Raw measures
  uint16_t _t;
  uint16_t _h;

  // Calibrated values
  float _temp;
  float _humi;
};

#endif
