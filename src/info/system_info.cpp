#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

// get_uptime returns a double from /proc/uptime representing the system uptime
double get_uptime() {
  ifstream uptime(PROC_ROOT "/uptime");
  double result;

  if (!uptime) {
    cerr << "Can't open uptime." << endl;
    exit(-1);
  }

  uptime >> result;

  uptime.close();
  return result;
}

// get_system_info returns a SystemInfo struct comprising information from all structs in /src/info
// It also counts the number of each type of thread and the number of running processes.
SystemInfo get_system_info() {
  SystemInfo si;

  si.num_user_threads = si.num_kernel_threads = si.num_threads = si.num_running = 0;

  si.load_average = get_load_average();
  si.cpus = get_cpu_info();
  si.processes = get_all_processes(PROC_ROOT);
  si.memory_info = get_memory_info();

  si.num_processes = si.processes.size();
  si.uptime = get_uptime();

  for (int i = 0; i < (int) si.processes.size(); i++) {
    if (si.processes[i].state == 'R')
      si.num_running++;

    for (int j = 0; j < (int) si.processes[i].threads.size(); j++) {
      if (si.processes[i].threads[j].is_thread()) {
        si.num_threads++;
        if (si.processes[i].threads[j].is_kernel_thread()) si.num_kernel_threads++;
        else if (si.processes[i].threads[j].is_user_thread()) si.num_user_threads++;
      }
    }
  }

  return si;
}
