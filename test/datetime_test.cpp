#include "gtest/gtest.h"
#include "spl/datetime.h"
#include <string>

using namespace std;
using namespace spl;

class DateTimeTest: public ::testing::Test {
};

TEST_F(DateTimeTest, Nilday) {
  string str = "0000-00-00";
  DateTime date(str);
  EXPECT_FALSE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
}

TEST_F(DateTimeTest, Sunday) {
  string str = "2013-10-13";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(0, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Monday) {
  string str = "2013-10-14";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(1, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Tuesday) {
  string str = "2013-10-15";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(2, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Wednesday) {
  string str = "2013-10-16";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(3, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Thursday) {
  string str = "2013-10-17";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(4, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Friday) {
  string str = "2013-10-18";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(5, date.GetWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Saturday) {
  string str = "2013-10-19";
  DateTime date(str);
  EXPECT_TRUE(date.IsValid());
  EXPECT_EQ(str, date.GetDateStr());
  EXPECT_EQ(6, date.GetWeekday()) << "date: " << str;
}
