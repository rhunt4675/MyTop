#include "formatting.h"
#include "gtest/gtest.h"

using namespace std;

TEST(Formatting, UptimeLessThan1Day) {
  EXPECT_EQ("20:15:09", uptimeToReadable(72909.1));
}

TEST(Formatting, UptimeBetween1and2Days) {
  EXPECT_EQ("1 day, 20:15:09", uptimeToReadable(86400 + 72909.1));
}

TEST(Formatting, UptimeMoreThan1Day) {
  EXPECT_EQ("2 days, 20:15:09", uptimeToReadable(2 * 86400.0 + 72909.1));
}

TEST(Formatting, UptimeHandlesZero) {
  EXPECT_EQ("00:00:00", uptimeToReadable(0));
}

TEST(Formatting, TimeLessThan1Sec) {
  EXPECT_EQ("00:00.10", ticksToReadable(10ULL));
}

TEST(Formatting, TimeLessThan1Min) {
  EXPECT_EQ("00:02.50", ticksToReadable(250ULL));
}

TEST(Formatting, TimeLessThan1Hour) {
  EXPECT_EQ("01:02.50", ticksToReadable(6250ULL));
}

TEST(Formatting, TimeMoreThan1Hour) {
  EXPECT_EQ("100h", ticksToReadable(36000000ULL));
}

TEST(Formatting, TimeHandlesZero) {
  EXPECT_EQ("00:00.00", ticksToReadable(0ULL));
}

TEST(Formatting, BytesHandlesZero) {
  EXPECT_EQ("0 KiB", bytesToReadable(0));
}

TEST(Formatting, BytesLessThan1MB) {
  EXPECT_EQ("776 KiB", bytesToReadable(776));
}

TEST(Formatting, BytesLessThan1GB) {
  EXPECT_EQ("85.59 MiB", bytesToReadable(87654));
}

TEST(Formatting, BytesLessThan1TB) {
  EXPECT_EQ("8.19 GiB", bytesToReadable(8593729));
}

TEST(Formatting, BytesHuge) {
  EXPECT_EQ("117.73 GiB", bytesToReadable(123456123));
}
