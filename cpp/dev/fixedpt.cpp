// https://blog.mbedded.ninja/programming/general/fixed-point-mathematics/
#include <cstdio>
#include <cmath>
#include "fastmath.hpp"


constexpr float A = 200.123456789f;
constexpr float B = -340.23456789f;

// probably use this instead?
// https://github.com/MikeLankamp/fpm/tree/master

int main() {
  int f = FP_FRAC; // fractional
  int i = 32-f; // mantisa
  // printf("Min value: %lf\n", -pow(2,i-1));
  // printf("Max value: %lf\n", pow(2,i-1)-1.0+pow(2,-f)*(pow(2,f)-1.0));
  // printf("fractional: %lf\n", pow(2, -f));
  fp_t<11> fp;
  printf("Min value: %lf\n", fp.MIN_FIXPT);
  printf("Max value: %lf\n", fp.MAX_FIXPT);
  printf("fractional precision: %lf\n", fp.SCALE);
  printf("------------------------------\n\n");
  printf("Min value: %lf\n", MIN_FIXPT);
  printf("Max value: %lf\n", MAX_FIXPT);
  printf("fractional precision: %lf\n", SCALE);
  printf("------------------------------\n\n");

  uint16_t ii = 324;
  fixpt iii = int2fixpt<uint16_t>(ii);
  if (ii == fixpt2int<uint16_t>(iii)) printf("fix2int works %d\n", (int)fixpt2int<uint16_t>(iii));

  fixpt a = float2fixpt(A);
  fixpt b = float2fixpt(B);
  float c;

  printf("float2fix: %f\n", fixpt2float(a) - A);
  printf("float2fix: %f\n", fixpt2float(b) - B);

  c = fixpt2float(multfixpt(a,b));
  printf("float mult: %f ** bad **\n", c - (A * B));

  c = fixpt2float(multfixpt(iii,b));
  printf("float/int mult: %f ** bad **\n", c - (ii * B));

  c = fixpt2float(a - iii);
  printf("float - int: %f\n", c - (A - 32455));

  c = fixpt2float(a + b);
  printf("float add: %f\n", c - (A + B));

  c = fixpt2float(a - b);
  printf("float sub: %f\n", c - (A - B));

  c = fixpt2float(divfixpt(a,b));
  printf("float div: %f\n", c - A / B);

  c = fixpt2float(sqrtfixpt(a));
  printf("float sqrt: %f\n", c - sqrt(A));
  return 0;
}