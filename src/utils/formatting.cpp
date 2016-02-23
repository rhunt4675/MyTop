#include "formatting.h"
#include <sstream>
#include <iomanip>
#include <cstdio>

// TString generates a 0-padded string of length 2 for use in a timestamp
string TString(int Num) {
  stringstream ss;
  ss << setfill('0') << setw(2) << Num;
  return ss.str();
}

// MinString generates a string without padding given an int/double
template <typename T> string MinString(T n) {
  stringstream ss;
  ss << n;
  return ss.str();
}

// bytesToReadable generates a human readable string given a quantity in bytes
string bytesToReadable(long bytes) {
  string size[] = {"B  ", "KiB", "MiB", "GiB"};

  int i(0);
  double newSize((double) bytes);

  while ((bytes /= 1024) && i < 3) {
    newSize /= 1024.0;
    i++;
  }

  char buffer[20];
  sprintf(buffer, "%.2f %s", newSize, size[i].c_str());
  return string(buffer);
}

// ticksToReadable generates a human readable timestamp string (HH:MM:SS.HH) given some number of ticks
string ticksToReadable(unsigned long long ticks) {
  int hrs = ticks / 360000;
  int mins = (ticks % 360000) / 6000;
  int secs = (ticks % 6000) / 100;
  int hdths = (ticks % 100);

  if (!hrs)
    return TString(mins) + ":" + TString(secs) + "." + TString(hdths);
  else if (!hdths && !secs && !mins)
    return TString(hrs) + "h";
  else if (!hdths && !secs)
    return TString(hrs) + ":" + TString(mins);
  else if (!hdths)
    return TString(hrs) + ":" + TString(mins) + ":" + TString(secs);
  //Default
  return TString(hrs) + ":" + TString(mins) + ":" + TString(secs) + "." + TString(hdths);
}

// uptimeToReadable generates a human readable timestamp string (XX days, HH:MM:SS) given an uptime double
string uptimeToReadable(double uptime) {
  int hdths = (int) (uptime * 100);
  int days = hdths / 8640000;
  int hrs = (hdths % 8640000) / 360000;
  int mins = (hdths % 360000) / 6000;
  int secs = hdths % 6000 / 100;

  return (!days ? "" : MinString(days) + (days == 1 ? " day, " : " days, ")) + TString(hrs) + ":" + TString(mins) + ":" + TString(secs);
}
