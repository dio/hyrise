#include "gtest/gtest.h"

#include "statistics/histograms/string_histogram_domain.hpp"

namespace opossum {

class StringHistogramDomainTest : public ::testing::Test {
 public:

  StringHistogramDomain domain_a{"abcdefghijklmnopqrstuvwxyz", 4u};
};

TEST_F(StringHistogramDomainTest, NextValue) {
  EXPECT_EQ(domain_a.next_value(""), "a");
  EXPECT_EQ(domain_a.next_value("a"), "aa");
  EXPECT_EQ(domain_a.next_value("ayz"), "ayza");
  EXPECT_EQ(domain_a.next_value("ayzz"), "az");
  EXPECT_EQ(domain_a.next_value("azzz"), "b");
  EXPECT_EQ(domain_a.next_value("z"), "za");
  EXPECT_EQ(domain_a.next_value("df"), "dfa");
  EXPECT_EQ(domain_a.next_value("abcd"), "abce");
  EXPECT_EQ(domain_a.next_value("abaz"), "abb");
  EXPECT_EQ(domain_a.next_value("abzz"), "ac");
  EXPECT_EQ(domain_a.next_value("abca"), "abcb");
  EXPECT_EQ(domain_a.next_value("abaa"), "abab");

  // Special case.
  EXPECT_EQ(domain_a.next_value("zzzz"), "zzzz");
}

TEST_F(StringHistogramDomainTest, StringToNumber) {
  EXPECT_EQ(domain_a.string_to_number(""), 0ul);

  // 0 * 26^3 + 1
  EXPECT_EQ(domain_a.string_to_number("a"), 1ul);

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.string_to_number("aa"), 2ul);

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^1 + 26^0) + 1 +
  // 0 * 26^0 + 1
  EXPECT_EQ(domain_a.string_to_number("aaaa"), 4ul);

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^1 + 26^0) + 1 +
  // 1 * 26^0 + 1
  EXPECT_EQ(domain_a.string_to_number("aaab"), 5ul);

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^1 + 26^0) + 1 +
  // 25 * 26^0 + 1
  EXPECT_EQ(domain_a.string_to_number("azzz"), 18'279ul);

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.string_to_number("b"), 18'280ul);

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.string_to_number("ba"), 18'281ul);

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 7 * (26^2 + 26^1 + 26^0) + 1 +
  // 9 * (26^1 + 26^0) + 1 +
  // 0 * 26^0 + 1
  EXPECT_EQ(domain_a.string_to_number("bhja"), 23'447ul);

  // 2 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 3 * (26^2 + 26^1 + 26^0) + 1 +
  // 4 * (26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.string_to_number("cde"), 38'778ul);

  // 25 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^1 + 26^0) + 1 +
  // 25 * 26^0 + 1
  EXPECT_EQ(domain_a.string_to_number("zzzz"), 475'254ul);
}

TEST_F(StringHistogramDomainTest, NumberToString) {
  EXPECT_EQ(domain_a.number_to_string(0ul), "");

  // 0 * 26^3 + 1
  EXPECT_EQ(domain_a.number_to_string(1ul), "a");

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.number_to_string(2ul), "aa");

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^1 + 26^0) + 1 +
  // 0 * 26^0 + 1
  EXPECT_EQ(domain_a.number_to_string(4ul), "aaaa");

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^1 + 26^0) + 1 +
  // 1 * 26^0 + 1
  EXPECT_EQ(domain_a.number_to_string(5ul), "aaab");

  // 0 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^1 + 26^0) + 1 +
  // 25 * 26^0 + 1
  EXPECT_EQ(domain_a.number_to_string(18'279ul), "azzz");

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.number_to_string(18'280ul), "b");

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 0 * (26^2 + 26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.number_to_string(18'281ul), "ba");

  // 1 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 7 * (26^2 + 26^1 + 26^0) + 1 +
  // 9 * (26^1 + 26^0) + 1 +
  // 0 * 26^0 + 1
  EXPECT_EQ(domain_a.number_to_string(23'447ul), "bhja");

  // 2 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 3 * (26^2 + 26^1 + 26^0) + 1 +
  // 4 * (26^1 + 26^0) + 1
  EXPECT_EQ(domain_a.number_to_string(38'778ul), "cde");

  // 25 * (26^3 + 26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^2 + 26^1 + 26^0) + 1 +
  // 25 * (26^1 + 26^0) + 1 +
  // 25 * 26^0 + 1
  EXPECT_EQ(domain_a.number_to_string(475'254ul), "zzzz");
}

TEST_F(StringHistogramDomainTest, NumberToStringBruteForce) {
  StringHistogramDomain domain{"abcd", 3u};

  EXPECT_EQ(domain.string_to_number("ddd"), 84);
  for (auto number = 0u; number < 84; number++) {
    EXPECT_LT(domain.number_to_string(number), domain.number_to_string(number + 1));
  }
}

TEST_F(StringHistogramDomainTest, StringToNumberBruteForce) {
  StringHistogramDomain domain{"abcd", 3u};

  EXPECT_EQ(domain.string_to_number("ddd"), 84);
  for (auto number = 0u; number < 84; number++) {
    EXPECT_EQ(domain.string_to_number(domain.number_to_string(number)), number);
  }
}

}  // namespace opossum
