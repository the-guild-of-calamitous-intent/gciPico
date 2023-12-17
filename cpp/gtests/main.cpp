#include <gtest/gtest.h>
#include <string>
#include "string.hpp"
#include "fastmath.hpp"
#include <iostream>
#include <vector>
#import <cmath>

using namespace std;

#define ABS_ERR 2.0f*powf(2,-16)

TEST(pico, math) {
  fixpt a = float2fixpt(20.123);
  fixpt b = float2fixpt(34.234);
  float c = fixpt2float(multfixpt(a,b));
  EXPECT_NEAR(c, 20.123 * 34.234, ABS_ERR);

  c = fixpt2float(a + b);
  EXPECT_FLOAT_EQ(c, 20.123 + 34.234);

  c = fixpt2float(a - b);
  EXPECT_FLOAT_EQ(c, 20.123 - 34.234);

  c = fixpt2float(divfixpt(a,b));
  EXPECT_FLOAT_EQ(c, 20.123 / 34.234);
}
