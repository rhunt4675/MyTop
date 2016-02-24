# Project 2

1. Ryan Hunt

2. src/main.cpp -- Runs refresh loop, draws information to the screen  
   src/info/cpu_info{.h,.cpp} -- Reads cpuinfo from PROC_ROOT and fills corresponding struct  
   src/info/load_average_info(.h,.cpp} -- Reads loadavg from PROC_ROOT and fills corresponding struct  
   src/info/memory_info{.h,.cpp} -- Reads meminfo from PROC_ROOT and fills corresponding struct  
   src/info/process_info{.h,.cpp} -- Reads all processes and threads in a given directory and fills a vector of structs  
   src/info/system_info{.h,.cpp} -- Compiles all vectors and structs into a larger struct, counts threads and processes  
   src/utils/formatting{.h,.cpp} -- Contains utility functions to generate human readable timestamps and data sizes
  
3. Displays owner of process -- ProcessInfo struct now holds appropriate UID  
   ASCII Art

4. ~15 hours

5. ~ 50 lines x 150 columns
