/**
 * Contains the main() routine of what will eventually be your version of top.
 */

#include <cstdlib>
#include <algorithm>
#include <map>
#include <cstring>
#include <ncurses.h>
#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include "info/system_info.h"
#include "utils/formatting.h"

using namespace std;

// Sort Functions
bool pidSort(ProcessInfo p1, ProcessInfo p2) { return p1.pid < p2.pid; }
bool cpuSort(ProcessInfo p1, ProcessInfo p2) { return p1.cpu_percent > p2.cpu_percent; }
bool memSort(ProcessInfo p1, ProcessInfo p2) { return p1.rss > p2.rss; }
bool timeSort(ProcessInfo p1, ProcessInfo p2) { return (p1.utime + p1.stime) > (p2.utime + p2.stime); }

// Misc CPU Calculation Functions
void setProcessUtilization(vector<ProcessInfo>&, map<int, ProcessInfo>&, unsigned long long period);
void fillMap(vector<ProcessInfo>&, map<int, ProcessInfo>&);

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}

// Utilizes getopt_long to parse user-provided arguments
void getFlags(int argc, char** argv, int* delay, int* sort, int* help) {
  static struct option long_options[] = {
    {   "delay", required_argument, NULL, 'd'},
    {"sort-key", required_argument, NULL, 's'},
    {    "help",       no_argument, NULL, 'h'},
    {         0,                 0,    0,   0}
  };

  while(true) {
    int option_index(0);
    int flag_char = getopt_long(argc, argv, "d:s:h", long_options, &option_index);

    if (flag_char == -1) break;

    switch(flag_char) {
      case 'd':
        if (atoi(optarg) < 0) {
          cerr << "Invalid value \"" << optarg << "\" for option delay." << endl;
          exit(-1);
        }
        *delay = atoi(optarg); break;

      case 's':
        if (strcmp(optarg, "PID") == 0) *sort = 0;
        else if (strcmp(optarg, "CPU") == 0) *sort = 1;
        else if (strcmp(optarg, "MEM") == 0) *sort = 2;
        else if (strcmp(optarg, "TIME") == 0) *sort = 3;
        else {
          cerr << "Invalid value \"" << optarg << "\" for option sort." << endl;
          exit(-1);
        }
        break;

      case 'h':
        cout << "MyTop v1.0 -- by Ryan Hunt" << endl << endl
             << "Usage: mytop -d DELAY -s SORT_COL [-h]" << endl
             << "	-d --delay DELAY 	(Delay between updates, in tenths of seconds)" << endl
             << "	-s --sort-key COLUMN 	(Sort by this column; one of: PID, CPU, MEM, TIME)" << endl
             << "	-h --help 		(Display a help message about these flags and exit)" << endl;
        exit(0);

      case '?':
       exit(-1);

      default:
        exit(-1);
    }
  }

  if (optind < argc) {
    cout << "Non-option ARGV-elements: ";
    for (int i = optind; i < argc; i++) {
      cout << argv[i] << " ";
    }
    cout << endl;
  }
}

/**
 * Entry point for the program.
 */
int main(int argc, char** argv) {
  int delay(10), sortMode(1), help(FALSE), row, col;
  long page_size = sysconf(_SC_PAGESIZE);

  // Parse flags with getoptlong
  getFlags(argc, argv, &delay, &sortMode, &help);

  // ncurses initialization
  initscr();
  getmaxyx(stdscr, row, col);

  // Don't show a cursor.
  curs_set(FALSE);

  // Set getch to return after delay * 100 milliseconds; this allows the program to
  // immediately respond to user input while not blocking indefinitely.
  timeout(delay * 100);

  // Array of sorters
  bool (*sorters[])(ProcessInfo, ProcessInfo) = {pidSort, cpuSort, memSort, timeSort};

  SystemInfo newSI = get_system_info();
  SystemInfo oldSI;
  map<int, ProcessInfo> oldProcessMap;

  while (true) {
    wclear(stdscr);

    //Cycle System Info Structs
    oldSI = newSI; newSI = get_system_info();

    //Begin Draw Screen ----------------------------------------------------------------------------------------------------------------------------------------
    double uptime = newSI.uptime;
    printw("                    __  __     _____\n                   |  \\/  |_  |_   _|__ _ __\n                   | |\\/| | || || |/ _ \\ '_ \\\n                   |_|  |_|\\_, ||_|\\___/ .__/\n                            |__/        |_|   \n\n");
    printw("                System Uptime: %s\n", uptimeToReadable(uptime).c_str());

    LoadAverageInfo lai = newSI.load_average;
    printw("                 Load Average: %4.2f, %4.2f, %4.2f\n\n", lai.one_min, lai.five_mins, lai.fifteen_mins);

    for (int i = 0; i < (int) newSI.cpus.size(); i++) {
      CpuInfo delta = newSI.cpus[i] - oldSI.cpus[i];
      double userTime = 100.0 * delta.user_time / delta.total_time();
      double kernelTime = 100.0 * delta.system_time / delta.total_time();
      double idleTime = 100.0 * delta.idle_time / delta.total_time();
      if (i == 0) {
        printw("Overall: %6.2f [User Time], %6.2f [Kernel Time], %6.2f [Idle Time]\n", userTime, kernelTime, idleTime);
      } else {
        printw("-->CPU%d: %6.2f [User Time], %6.2f [Kernel Time], %6.2f [Idle Time]\n", i, userTime, kernelTime, idleTime);
      }
    }

    MemoryInfo mi = newSI.memory_info;

    printw("\n\
Number of Processes:     %4d            Installed Memory: %10s\n\
Running Processes:       %4d            Used Memory:      %10s\n\
Number of Threads: %3dU, %3dK            Available Memory: %10s\n\n", newSI.num_processes, bytesToReadable(mi.total_memory * 1024).c_str(),
        newSI.num_running,  bytesToReadable((mi.total_memory - mi.free_memory) * 1024).c_str(), newSI.num_user_threads, newSI.num_kernel_threads, bytesToReadable(mi.free_memory * 1024).c_str());

    CpuInfo diff = newSI.cpus[1] - oldSI.cpus[1];
    setProcessUtilization(newSI.processes, oldProcessMap, diff.total_time());
    fillMap(newSI.processes, oldProcessMap);

    sort(newSI.processes.begin(), newSI.processes.end(), (*sorters[sortMode]));

    printw("---------------------------------------------------------------------\n");
    printw("|S|  PID  |    Size    | CPU |  CPU Time  |  User  |     Binary     |\n");
    printw("---------------------------------------------------------------------\n");
    for (int i = 0; i < (int)newSI.processes.size() && i < row - 26; i++) {
      printw(" %c  %5d  %11s %5.1f %12s  %8s  %s\n", newSI.processes[i].state, newSI.processes[i].pid,
        bytesToReadable(newSI.processes[i].rss * page_size).c_str(), newSI.processes[i].cpu_percent,
        ticksToReadable(newSI.processes[i].stime + newSI.processes[i].utime).c_str(), getpwuid(newSI.processes[i].uid)->pw_name,
        string(newSI.processes[i].comm).substr(1, strlen(newSI.processes[i].comm) - 2).c_str());
    }
    //End Draw Screeen ----------------------------------------------------------------------------------------------------------------------------------------

    // Redraw the screen.
    refresh();

    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}

// setProcessUtilization computes the utilization of each process from a map of previous system information data
void setProcessUtilization(vector<ProcessInfo>& processes, map<int, ProcessInfo>& prevProcesses, unsigned long long period) {
  for (int i = 0; i < (int)processes.size(); i++) {
    if (prevProcesses.find(processes[i].pid) == prevProcesses.end())
      processes[i].cpu_percent = 0.0;
    else {
      ProcessInfo oldSnapshot = prevProcesses[processes[i].pid];
      processes[i].cpu_percent = (100.0 * (processes[i].stime + processes[i].utime - oldSnapshot.stime - oldSnapshot.utime)) / period;
    }
  }
}

// fillMap generates a map of PIDs to ProcessInfo structs given an array of ProcessInfo structs
void fillMap(vector<ProcessInfo>& processes, map<int, ProcessInfo>& oldMap) {
  oldMap.clear();

  for (int i = 0; i < (int)processes.size(); i++)
    oldMap[processes[i].pid] = processes[i];
}
