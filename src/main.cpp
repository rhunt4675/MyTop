/**
 * Contains the main() routine of what will eventually be your version of top.
 */

#include <cstdlib>
#include <algorithm>

#include <iostream>

#include <ncurses.h>
#include "info/system_info.h"
#include "utils/formatting.h"

using namespace std;

bool pidSort(ProcessInfo p1, ProcessInfo p2) { return p1.pid < p2.pid; }
bool cpuSort(ProcessInfo p1, ProcessInfo p2) { return p1.cpu_percent > p2.cpu_percent; }
bool memSort(ProcessInfo p1, ProcessInfo p2) { return p1.size > p2.size; }
bool timeSort(ProcessInfo p1, ProcessInfo p2) { return (p1.utime + p1.stime) > (p2.utime + p2.stime); }

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


/**
 * Entry point for the program.
 */
int main() {
  // ncurses initialization
  initscr();

  // Don't show a cursor.
  curs_set(FALSE);

  // Set getch to return after 1000 milliseconds; this allows the program to
  // immediately respond to user input while not blocking indefinitely.
  timeout(1000);

  bool (*sorters[])(ProcessInfo, ProcessInfo) = {pidSort, cpuSort, memSort, timeSort};

  SystemInfo newSI = get_system_info();
  SystemInfo oldSI;

  while (true) {
    wclear(stdscr);

    //Cycle System Info Structs
    oldSI = newSI; newSI = get_system_info();

    //Draw Screen ------------------------------------
    double uptime = newSI.uptime;
    printw("MyTop - System Uptime: %s\n", uptimeToReadable(uptime).c_str());

    LoadAverageInfo lai = newSI.load_average;
    printw("        Load Average: %4.2f, %4.2f, %4.2f\n\n", lai.one_min, lai.five_mins, lai.fifteen_mins);

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
Number of Processes: %4d                Installed Memory: %10s\n\
Running Processes:   %4d                Used Memory:      %10s\n\
Number of Threads:   %4d                Available Memory: %10s\n\n", newSI.num_processes, bytesToReadable(mi.total_memory).c_str(),
        newSI.num_running,  bytesToReadable(mi.total_memory - mi.free_memory).c_str(), newSI.num_threads, bytesToReadable(mi.free_memory).c_str());

    sort(newSI.processes.begin(), newSI.processes.end(), (*sorters[2]));

    printw("-------------------------------------------------------------\n");
    printw("|  PID  |     Size     | State |  %%CPU  |  CPU Time  |  Binary  \n");
    printw("-------------------------------------------------------------\n");
    for (int i = 0; i < (int)newSI.processes.size(); i++) {
//      int pid = newSI.processes[i].pid;
      printw("%7d    %10s       %c     %8s %12s  %s\n", newSI.processes[i].pid, bytesToReadable(newSI.processes[i].size).c_str(), newSI.processes[i].state, "", 
	ticksToReadable(newSI.processes[i].stime + newSI.processes[i].utime).c_str(), newSI.processes[i].command_line.c_str());
    }
    //End Draw Screeen --------------------------------

    // Redraw the screen.
    refresh();

    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}
