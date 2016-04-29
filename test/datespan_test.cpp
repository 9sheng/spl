#include "gtest/gtest.h"
#include "spl/datespan.h"
#include <vector>

using namespace std;
using namespace spl;

class DateSpanTest: public ::testing::Test {
};

TEST_F(DateSpanTest, Normalize) {
  DateSpan dp("2013-10-14", "2013-10-18", "237");
  dp.Normalize();
  EXPECT_EQ("2013-10-15|2013-10-17|23", dp.ToString());

  dp.set_period("2");
  dp.Normalize();
  EXPECT_EQ("2013-10-15|2013-10-16|2", dp.ToString());

  dp.set_period("34");
  dp.Normalize();
  EXPECT_EQ("0000-00-00|0000-00-00|", dp.ToString());

  DateSpan dp2("2014-02-04", "2014-02-17", "1");
  dp2.Normalize();
  EXPECT_EQ("2014-02-10|2014-02-11|1", dp2.ToString());
}

TEST_F(DateSpanTest, ShiftDays) {
  DateSpan dp("2013-10-13", "2013-10-17", "1237");
  dp.ShiftDays(2);

  EXPECT_EQ("2013-10-15|2013-10-19|2345", dp.ToString());
  dp.ShiftDays(-2);
  EXPECT_EQ("2013-10-13|2013-10-17|1237", dp.ToString());
}

TEST_F(DateSpanTest, Merge) {
  DateSpan dp1("2014-05-26", "2014-05-27", "1");
  DateSpan dp2("2014-05-25", "2014-05-26", "7");

  vector<DateSpan> in;
  vector<DateSpan> out;

  in.push_back(dp2);
  in.push_back(dp1);

  DateSpan::Merge(in, &out);
  ASSERT_EQ((size_t)1, out.size());
  EXPECT_EQ("2014-05-25|2014-05-27|17", out[0].ToString());
}

TEST_F(DateSpanTest, Merge2) {
  DateSpan dp1("2014-01-08", "2014-01-25", "1234567");
  DateSpan dp2("2014-01-25", "2014-01-26", "6");

  std::vector<DateSpan> in;
  std::vector<DateSpan> out;

  in.push_back(dp1);
  in.push_back(dp2);

  DateSpan::Merge(in, &out);
  ASSERT_EQ((size_t)1, out.size());
  EXPECT_EQ("2014-01-08|2014-01-26|1234567", out[0].ToString());
}

TEST_F(DateSpanTest, Merge3) {
  DateSpan dp1("2014-01-08", "2014-01-24", "123456");
  DateSpan dp2("2014-01-12", "2014-03-24", "7");
  DateSpan dp3("2014-01-24", "2014-01-25", "5");
  DateSpan dp4("2014-01-25", "2014-03-30", "123456");

  std::vector<DateSpan> in;
  std::vector<DateSpan> out;

  in.push_back(dp1);
  in.push_back(dp2);
  in.push_back(dp3);
  in.push_back(dp4);

  DateSpan::Merge(in, &out);

  ASSERT_EQ((size_t)2, out.size());
  EXPECT_EQ("2014-01-08|2014-01-12|3456", out[0].ToString());
  EXPECT_EQ("2014-01-12|2014-03-30|1234567", out[1].ToString());
}

TEST_F(DateSpanTest, Merge4) {
  DateSpan dp1("2013-12-29", "2013-12-30", "7");
  DateSpan dp2("2013-12-30", "2013-12-31", "1");
  DateSpan dp3("2014-01-01", "2014-01-02", "3");
  DateSpan dp4("2014-01-05", "2014-03-27", "37");
  DateSpan dp5("2014-01-09", "2014-01-10", "4");
  DateSpan dp6("2014-01-13", "2014-03-29", "145");

  std::vector<DateSpan> in;
  std::vector<DateSpan> out;

  in.push_back(dp1);
  in.push_back(dp2);
  in.push_back(dp3);
  in.push_back(dp4);
  in.push_back(dp5);
  in.push_back(dp6);

  DateSpan::Merge(in, &out);
  ASSERT_EQ((size_t)4, out.size());
  EXPECT_EQ("2013-12-29|2014-01-02|137", out[0].ToString());
  EXPECT_EQ("2014-01-05|2014-01-09|37", out[1].ToString());
  EXPECT_EQ("2014-01-09|2014-01-13|47", out[2].ToString());
  EXPECT_EQ("2014-01-13|2014-03-29|13457", out[3].ToString());
}

TEST_F(DateSpanTest, SamePeriod) {
  DateSpan dp1("2013-10-13", "2013-10-17", "1237");
  DateSpan dp2("2013-10-20", "2013-10-28", "1237");

  vector<DateSpan> in;
  vector<DateSpan> out;

  in.push_back(dp2);
  in.push_back(dp1);

  DateSpan::Merge(in, &out);
  ASSERT_EQ((size_t)1, out.size());
  EXPECT_EQ("2013-10-13|2013-10-28|1237", out[0].ToString());
}

TEST_F(DateSpanTest, Intersect) {
  DateSpan dp1("2013-10-14", "2013-10-19", "2357");
  DateSpan dp2("2013-10-13", "2013-10-27", "12347");
  DateSpan dp3 = dp1.Intersect(dp2);
  EXPECT_EQ("2013-10-14|2013-10-19|237", dp3.ToString());

  DateSpan dp4 = dp1.Intersect(dp3);
  EXPECT_EQ("2013-10-14|2013-10-19|237", dp4.ToString());

  DateSpan dp5("2013-10-20", "2013-10-30", "2357");
  DateSpan dp6 = dp4.Intersect(dp5);
  EXPECT_EQ("0000-00-00|0000-00-00|", dp6.ToString());
}
