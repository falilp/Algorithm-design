#ifndef CHRONOMETER_H_ 
#define CHRONOMETER_H_

#include <ctime>
using std::clock;
using std::clock_t;

const double pps = CLOCKS_PER_SEC;

class chronometer {
public:
  chronometer(): active(false) {}
  void start() { active = true; t0 = clock(); }
  void stop() { if (active) { t1 = clock(); active = false; } }
  double time() const { return ((active ? clock() : t1) - t0) / pps; }
private:
  bool active;
  clock_t t0, t1;
};

#endif
