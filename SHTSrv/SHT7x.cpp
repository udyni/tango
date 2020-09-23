// kate: replace-tabs on; indent-width 2; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 2
/*
 * Sensirion SHT7* interface for Raspberry Pi with wiringPi
 */

#include "SHT7x.h"

const char sht7x_crc_lut[256] = {0, 49, 98, 83, 196, 245, 166, 151, 185, 136, 219,
234, 125, 76, 31, 46, 67, 114, 33, 16, 135, 182, 229, 212, 250, 203, 152, 169, 62,
15, 92, 109, 134, 183, 228, 213, 66, 115, 32, 17, 63, 14, 93,108, 251, 202, 153,
168, 197, 244, 167, 150, 1, 48, 99, 82, 124, 77, 30, 47, 184, 137, 218, 235, 61,
12, 95, 110, 249, 200, 155, 170, 132, 181, 230, 215, 64, 113, 34, 19, 126, 79, 28,
45, 186, 139, 216, 233, 199, 246, 165, 148, 3, 50, 97, 80, 187, 138, 217, 232, 127,
78, 29, 44, 2, 51, 96, 81, 198, 247, 164, 149, 248, 201, 154, 171, 60, 13, 94, 111,
65, 112, 35, 18, 133, 180, 231, 214, 122, 75, 24, 41, 190, 143, 220, 237, 195, 242,
161, 144, 7, 54, 101, 84, 57, 8, 91, 106, 253, 204, 159, 174, 128, 177, 226, 211,
68, 117, 38, 23, 252, 205, 158, 175, 56, 9, 90, 107, 69, 116, 39, 22, 129, 176, 227,
210, 191, 142, 221, 236, 123, 74, 25, 40, 6, 55, 100, 85, 194, 243, 160, 145, 71, 118,
37, 20, 131, 178, 225, 208, 254, 207, 156, 173, 58, 11, 88, 105, 4, 53, 102, 87, 192,
241, 162, 147, 189, 140, 223, 238, 121, 72, 27, 42, 193, 240, 163, 146, 5, 52, 103,
86, 120, 73, 26, 43, 188, 141, 222, 239, 130, 179, 224, 209, 70, 119, 36, 21, 59, 10,
89, 104, 255, 206, 157, 172};


// Initialize sensor interface
void SensirionSHT::init() {
  // Set pin mode
  pinMode(_sck, OUTPUT);
  // Wait 11ms
  delay(11);
  // Reset
  connReset();
  // Wait another 11ms
  delay(11);
}

// Start a T and H measure
int SensirionSHT::measure() {
  if(measureValue(_t, MEASURE_TEMP))
    return -1;
  if(measureValue(_h, MEASURE_HUMI))
    return -1;
  _temp = computeT();
  _humi = computeH();
  return 0;
}

// Compute dew point
float SensirionSHT::computeD(float t, float h) {
  const float Tn_1 = 243.12;
  const float m_1  =  17.62;
  const float Tn_2 = 272.62;
  const float m_2  =  22.46;

  h = log(h / 100.0);

  float dp = 0.0;
  if(t > 0) {
    float rt = (m_1*t)/(Tn_1 + t);
    dp = Tn_1 * (h + rt) / (m_1 - h - rt);
  } else {
    float rt = (m_2*t)/(Tn_2 + t);
    dp = Tn_2 * (h + rt) / (m_2 - h - rt);
  }
  return dp;
}

// Send a start sequence
void SensirionSHT::trStart() {
  // Grab data pin
  pinMode(_sda, OUTPUT);
  digitalWrite(_sda, HIGH);
  digitalWrite(_sck, LOW);
  delayMicroseconds(1 * DELAY_MULTIPLIER);

  digitalWrite(_sck, HIGH);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  digitalWrite(_sda, LOW);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  digitalWrite(_sck, LOW);
  delayMicroseconds(5 * DELAY_MULTIPLIER);
  digitalWrite(_sck, HIGH);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  digitalWrite(_sda, HIGH);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  digitalWrite(_sck, LOW);

  // Release data pin
  pinMode(_sda, INPUT);
}

// Send one byte to sensor
int SensirionSHT::sendByte(uint8_t b) {
  // Set data as output
  pinMode(_sda, OUTPUT);
  // Set data bits
  for(uint8_t i=0x80; i>0; i/=2) {
    if(b & i)
      digitalWrite(_sda, HIGH);
    else
      digitalWrite(_sda, LOW);
    // Wait setup time
    delayMicroseconds(1 * DELAY_MULTIPLIER);
    digitalWrite(_sck, HIGH);
    // Pulse width
    delayMicroseconds(5 * DELAY_MULTIPLIER);
    digitalWrite(_sck, LOW);
  }
  // Release data pin
  pinMode(_sda, INPUT);
  digitalWrite(_sda, HIGH);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  // Get ack
  digitalWrite(_sck, 1);
  int error = digitalRead(_sda);
  digitalWrite(_sck, 0);
  return error;
}

// Receive one byte from sensor
uint8_t SensirionSHT::recvByte(uint8_t ack) {
  // Release data pin
  pinMode(_sda, INPUT);
  digitalWrite(_sda, HIGH);

  uint8_t val = 0;
  for(uint8_t i=0x80; i>0; i/=2) {
    digitalWrite(_sck, HIGH);
    if(digitalRead(_sda) == HIGH)
      val |= i;
    digitalWrite(_sck, LOW);
  }

  // Send ack
  pinMode(_sda, OUTPUT);
  if(ack)
    digitalWrite(_sda, LOW);
  else
    digitalWrite(_sda, HIGH);
  delayMicroseconds(1 * DELAY_MULTIPLIER);
  digitalWrite(_sck, HIGH);
  delayMicroseconds(5 * DELAY_MULTIPLIER);
  digitalWrite(_sck, LOW);

  // Release data line
  pinMode(_sda, INPUT);

  return val;
}

// Reset connection
void SensirionSHT::connReset() {
  // Reset connection. At least 9 SCK cycles while sda is HIGH
  pinMode(_sda, OUTPUT);
  digitalWrite(_sda, HIGH);
  digitalWrite(_sck, LOW);
  // Nine sck pulses
  for(uint8_t i=0; i<9; i++) {
    digitalWrite(_sck, HIGH);
    delayMicroseconds(1 * DELAY_MULTIPLIER);
    digitalWrite(_sck, LOW);
  }
  trStart();
}

// Measure value
int SensirionSHT::measureValue(uint16_t& val, uint8_t cmd) {
  // Start
  trStart();
  // Send measure command
  sendByte(cmd);
  pinMode(_sda, INPUT);
  // Wait for ready signal from sensor
  int count = 0;
  while(1) {
    if(digitalRead(_sda) == LOW)
      break;
    delay(4);
    if(count > 50)
      return -1;
  }
  uint16_t hi = recvByte(1);
  uint16_t lo = recvByte(1);
  uint8_t chksum = recvByte(0);

  // Compute checksum
  uint8_t check = computeCRC(cmd, hi, lo);
  //printf("Checksum: 0x%02X <-> 0x%02X\n", chksum, check);

  // Check checksum
  if(check != chksum)
      return -1;

  // Compose value
  val = lo | (hi << 8);
  return 0;
}

// Compute CRC
uint8_t SensirionSHT::computeCRC(uint8_t cmd, uint8_t hi, uint8_t lo) {
  uint8_t crc = 0;
  crc = sht7x_crc_lut[cmd xor crc];
  crc = sht7x_crc_lut[(uint8_t)hi xor crc];
  crc = sht7x_crc_lut[(uint8_t)lo xor crc];
  return reverse(crc);
}

// Reverse a byte bitwise
uint8_t SensirionSHT::reverse(uint8_t b) {
  uint8_t out = 0;
  for(size_t i = 0; i < 8; i++)
    out |= (b & (0x1 << i)) ? (0x1 << (7-i)) : 0;
  return out;
}

// Get temperature compensated relative humidity
float SensirionSHT::computeH() {
  const float C1=-2.0468;
  const float C2=+0.0367;
  const float C3=-0.0000015955;
  const float T1=+0.01;
  const float T2=+0.00008;

  float rh = float(_h);
  float rh_lin = C3*rh*rh + C2*rh + C1;
  float rh_true = (getT() - 25.0) * (T1+T2*rh) + rh_lin;

  if(rh_true>100) rh_true=100;   //cut if the value is outside of
  if(rh_true<0.1) rh_true=0.1;   //the physical possible range
  return rh_true;
}
