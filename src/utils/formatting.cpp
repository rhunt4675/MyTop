#include "formatting.h"
#include <sstream>
#include <iomanip>

string TString(int Num) {
  stringstream ss;
  ss << setfill('0') << setw(2) << Num;
  return ss.str();
}

template <typename T> string MinString(T n) {
  stringstream ss;
  ss << n;
  return ss.str();
}

string bytesToReadable(long bytes) {
  string size[] = {"KiB", "MiB", "GiB"};

  int i(0);
  double newSize((double) bytes);

  while ((bytes /= 1024) && i < 2) {
    newSize /= 1024.0;
    i++;
  }

  return MinString((int)(newSize * 100) / 100.0) + " " + size[i];
}

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

string uptimeToReadable(double uptime) {
  int hdths = (int) (uptime * 100);
  int days = hdths / 8640000;
  int hrs = (hdths % 8640000) / 360000;
  int mins = (hdths % 360000) / 6000;
  int secs = hdths % 6000 / 100;

  return (!days ? "" : MinString(days) + (days == 1 ? " day, " : " days, ")) + TString(hrs) + ":" + TString(mins) + ":" + TString(secs);
}
