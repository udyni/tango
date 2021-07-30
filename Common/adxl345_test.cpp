// kate: replace-tabs on; indent-width 2; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 2;

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <adxl345.h>
 #include <time.h>

using namespace std;

#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif


ADXL345* acc = NULL;

void wait_for_fifo() {
  struct timespec slp;
  slp.tv_sec = 0;
  slp.tv_nsec = 500000;
  while(true) {
    nanosleep(&slp, NULL);
    uint8_t reg = acc->getRegister(ADXL_REG_FIFO_STATUS);
    if(int(reg & 0x3F) > 1)
      break;
  }
}

int main() {

  cout << "ADXL345 Test program" << endl;

  acc = new ADXL345(1, ADXL345_ALT_ADDRESS);
  uint8_t reg = 0;

  // Set datarate
  acc->setDataRate(ADXL_RATE_400HZ);
  acc->setFifoMode(ADXL_FIFO_STREAM);
  acc->Start();

  struct timespec slp;
  slp.tv_sec = 0;
  slp.tv_nsec = 1000000;

  int count = 0;

  wait_for_fifo();

  while(count < 1200) {
    float x = 0, y = 0, z = 0;
    int fifo = 0;
    acc->getAcceleration(x, y, z, &fifo);
    cout << " - Acceleration: X = " << x << ", Y = " << y << ", Z = " << z << " (FIFO: " << fifo << ")" << endl;
    if(fifo == 0) {
      wait_for_fifo();
    } else {
      nanosleep(&slp, NULL);
    }
    count++;
  }

  delete acc;
  return 0;
}
