#include "memory_info.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

const static int NUM_TARGETS = 6;


MemoryInfo get_memory_info() {
  ifstream inFile(PROC_ROOT "/meminfo");
  MemoryInfo mi;
  string tmp;

  if (!inFile) {
  	cerr << "Can't open meminfo." << endl;
  	exit(-1);
  }

  // Targets to find in /proc/meminfo -- listed in order of position in file
  string targets[NUM_TARGETS] = {"MemTotal:", "MemFree:", "Buffers:", "Cached:", "SwapTotal:", "SwapFree:"};
  unsigned long long *writeToStruct[NUM_TARGETS] = {&mi.total_memory, &mi.free_memory, &mi.buffers_memory, &mi.cached_memory, &mi.total_swap, &mi.free_swap};

  for (int i = 0; i < NUM_TARGETS; i++) {
  	inFile >> tmp;

  	if (tmp != targets[i]) {
  		inFile >> tmp >> tmp;
  		i--; continue;
  	}

  	inFile >> *writeToStruct[i] >> tmp;
  }

  inFile.close();
  return mi;
}
