// kate: replace-tabs on; indent-width 2; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 2;

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <adxl355.h>
#include <time.h>

using namespace std;

#ifndef ELAPSED_TIME_MS
#define ELAPSED_TIME_MS(p, l) ( ((l.tv_sec) - (p.tv_sec)) * 1000 + ((l.tv_usec) - (p.tv_usec)) / 1000 )
#endif


ADXL355* acc = NULL;


int main() {

  cout << "ADXL355 Test program" << endl;

  acc = new ADXL355(1, ADXL355_ALT_ADDRESS);
  acc->setDataRate(ADXL355_ODR_1000);
  acc->Start();

  uint8_t samples = 0;
  struct timeval s, e;
  struct timespec sl = {0, 1000000};
  gettimeofday(&s, NULL);
  while(true) {
    samples = acc->getFifoSamples();
//     cout << "FIFO: " << int(samples) << endl;

    if(samples > 0) {
      double x, y, z;
      acc->getAcceleration(x, y, z);
      cout << "ACCELERATION: X(" << x << "), Y(" << y << "), Z(" << z << ")" << endl;
    } else {
      nanosleep(&sl, NULL);
    }

    gettimeofday(&e, NULL);
    int elapsed = ELAPSED_TIME_MS(s, e);
    if(elapsed > 1000)
      break;
  }


  for(size_t i = 0; i < 10; i++) {
    double t = acc->getTemperature();
    cout << "Temperature: " << t << " degC" << endl;
    sleep(1);
  }

  cout << "Done!" << endl;
  acc->Stop();
  delete acc;
  return 0;
}
