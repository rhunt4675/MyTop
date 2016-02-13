#include "cpu_info.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


vector<CpuInfo> get_cpu_info() {
  ifstream inFile(PROC_ROOT "/stat");

  vector<CpuInfo> cpus;
  string cpuName;
  CpuInfo ci;

  if (!inFile) {
  	cerr << "Can't open stat." << endl;
  	exit(-1);
  }

  while(true) {
  	inFile >> cpuName;
  	if (cpuName.substr(0, 3) != "cpu")
  		break;
  	inFile >> ci.user_time >> ci.nice_time >> ci.system_time
  		>> ci.idle_time >> ci.io_wait_time >> ci.irq_time
  		>> ci.softirq_time >> ci.steal_time >> ci.guest_time >> ci.guest_nice_time;

  	cpus.push_back(ci);
  }

  inFile.close();
  return cpus;
}


CpuInfo operator -(const CpuInfo& lhs, const CpuInfo& rhs) {
  CpuInfo result;

  result.user_time = lhs.user_time - rhs.user_time;
  result.nice_time = lhs.nice_time - rhs.nice_time;
  result.system_time = lhs.nice_time - rhs.nice_time;
  result.idle_time = lhs.idle_time - rhs.idle_time;
  result.io_wait_time = lhs.io_wait_time - rhs.io_wait_time;
  result.irq_time = lhs.irq_time - rhs.irq_time;
  result.softirq_time = lhs.softirq_time - rhs.softirq_time;
  result.steal_time = lhs.steal_time - rhs.steal_time;
  result.guest_time = lhs.guest_time - rhs.guest_time;
  result.guest_nice_time = lhs.guest_nice_time - rhs.guest_nice_time;

  return result;
}
