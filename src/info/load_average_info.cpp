#include "load_average_info.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

// get_load_average reads /proc/loadavg and returns a LoadAverageInfo struct containing information about system load
LoadAverageInfo get_load_average() {
  ifstream inFile(PROC_ROOT "/loadavg");
  LoadAverageInfo lai;

  if (!inFile) {
  	cerr << "Can't open loadavg." << endl;
  	exit(-1);
  }

  inFile >> lai.one_min >> lai.five_mins >> lai.fifteen_mins;

  inFile.close();
  return lai;
}
