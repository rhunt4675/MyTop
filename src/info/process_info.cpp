#include "process_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

// get_process retruns an individual ProcessInfo struct given a basedir and pid
// This function is extensible to functions and threads alike.
ProcessInfo get_process(int pid, const char* basedir) {
  ProcessInfo pi;
  string tmp;
  DIR* taskDir;

  stringstream ss;
  ss << basedir << "/" << pid;
  string root = ss.str();

  ifstream statm(string(root + "/statm").c_str());
  ifstream stat(string(root + "/stat").c_str());
  ifstream status(string(root + "/status").c_str());
  ifstream cmdline(string(root + "/cmdline").c_str());

  if (!statm || !stat || !status || !cmdline) {
    //Looks like the process has disppeared since get_process was spawned
    //Return invalid ProcessInfo struct
    pi.pid = -1; return pi;
  }

  statm >> pi.size >> pi.resident >> pi.share >> pi.trs >> pi.lrs >> pi.drs >> pi.dt;
  stat >> pi.pid >> pi.comm >> pi.state >> pi.ppid >> pi.pgrp >> pi.session >> pi.tty_nr
  	 >> pi.tpgid >> pi.flags >> pi.minflt >> pi.cminflt >> pi.majflt >> pi.cmajflt >> pi.utime
  	 >> pi.stime >> pi.cutime >> pi.cstime >> pi.priority >> pi.nice >> pi.num_threads
  	 >> pi.itrealvalue >> pi.starttime >> pi.vsize >> pi.rss >> pi.rsslim >> pi.startcode
  	 >> pi.endcode >> pi.startstack >> pi.kstkesp >> pi.kstkeip >> pi.signal >> pi.blocked
  	 >> pi.sigignore >> pi.sigcatch >> pi.wchan >> pi.nswap >> pi.cnswap >> pi.exit_signal
  	 >> pi.processor >> pi.rt_priority >> pi.policy >> pi.delayacct_blkio_ticks
  	 >> pi.guest_time >> pi.cguest_time;
  status >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> pi.tgid;

  getline(cmdline, pi.command_line);

  // Get full command string -- default to pi.comm if null
  for (int i = 0; i < (int) pi.command_line.length(); i++)
    if (pi.command_line[i] == '\0')
      pi.command_line[i] = ' ';
  pi.command_line = pi.command_line.substr(0, pi.command_line.length() - 1);
  if (pi.command_line.length() == 0)
    pi.command_line = string(pi.comm).substr(1, strlen(pi.comm) - 2);

  if ((taskDir = opendir(string(root + "/task/").c_str())) && !closedir(taskDir))
    pi.threads = get_all_processes(string(root + "/task").c_str());
  else {
    pi.threads = vector<ProcessInfo>();
  }

  statm.close();
  stat.close();
  status.close();
  cmdline.close();
  return pi;
}

// get_all_processes scans a given basedir for all process/thread directories and returns a vector describing them
// This is extensible to a system (/proc) directory and a process' task directory of threads
vector<ProcessInfo> get_all_processes(const char* basedir) {
  vector<ProcessInfo> vpi;
  DIR* pDir = NULL;
  dirent* pDirent = NULL;
  int pid;

  pDir = opendir(basedir);
  if (!pDir) {
    perror("Opening directory");
    exit(-1);
  }

  while ((pDirent = readdir(pDir))) {
    if (!pDirent) {
      perror("Reading directory");
      exit(-1);
    }
    if ((pid = strtol(pDirent->d_name, NULL, 10)) != 0L) {
      ProcessInfo pi = get_process(pid, basedir);
      if (pi.pid != -1) vpi.push_back(pi);
    }
  }

  if (closedir(pDir) < 0) {
    perror("Closing directory");
    exit(-1);
  }

  return vpi;
}
