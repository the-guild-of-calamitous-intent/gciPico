
#pragma once

#include <cstdint>
#include <cmath>

/*
# Pico SDK

pico_float

Part of: Runtime Infrastructure
Optimized single-precision floating point functions

(Replacement) optimized implementations are provided of the following compiler built-ins and math library functions:

__aeabi_fadd, __aeabi_fdiv, __aeabi_fmul, __aeabi_frsub, __aeabi_fsub, __aeabi_cfcmpeq, __aeabi_cfrcmple, __aeabi_cfcmple, __aeabi_fcmpeq, __aeabi_fcmplt, __aeabi_fcmple, __aeabi_fcmpge, __aeabi_fcmpgt, __aeabi_fcmpun, __aeabi_i2f, __aeabi_l2f, __aeabi_ui2f, __aeabi_ul2f, __aeabi_f2iz, __aeabi_f2lz, __aeabi_f2uiz, __aeabi_f2ulz, __aeabi_f2d, sqrtf, cosf, sinf, tanf, atan2f, expf, logf

ldexpf, copysignf, truncf, floorf, ceilf, roundf, asinf, acosf, atanf, sinhf, coshf, tanhf, asinhf, acoshf, atanhf, exp2f, log2f, exp10f, log10f, powf, hypotf, cbrtf, fmodf, dremf, remainderf, remquof, expm1f, log1pf, fmaf

powintf, sincosf (GNU extensions)

The following additional optimized functions are also provided:

fix2float, ufix2float, fix642float, ufix642float, float2fix, float2ufix, float2fix64, float2ufix64, float2int, float2int64, float2int_z, float2int64_z
*/

// # Fixed Point
// pico native it size is 32bits, so want something of that size
//
// fixpt is broken up into: integer bits and fractional bits
// - integer: includes the sign bit, so (FP_INT-1) on range
// - fractional: sets the precision or scaling value (how small are the increments)
//
// ## References:
// https://spin.atomicobject.com/2012/03/15/simple-fixed-point-math/
// https://github.com/MikeLankamp/fpm/tree/master

#define FP_INT 21
#define FP_FRAC (32-FP_INT)

// This is a simple pow function and doesn't handle negative values, so
// just do 1.0/ipow(a,b) for that. Also you can't use pow() for constexpr
constexpr double ipow(double num, uint32_t pow) {
    return (pow == 0 ? 1.0 : num * ipow(num, pow-1));
}
constexpr float SCALE = 1.0/ipow(2, FP_FRAC);
constexpr float MIN_FIXPT = -ipow(2, FP_INT-1);
constexpr double MAX_FIXPT = ipow(2, FP_INT-1) - SCALE;

typedef int32_t fixpt; // fp32_t or fp_t?
typedef int32_t fp32_t; // normal math
typedef int64_t fp64_t; // used to prevent overflow in mult/div

constexpr fixpt multfixpt(const fixpt a, const fixpt b) {
  // return (fixpt)(((int64_t)a*(int64_t)b)>>FP_FRAC);
  return ((int64_t)a * (int64_t)b) / (1 << FP_FRAC);
}
constexpr fixpt divfixpt(const fixpt a, const fixpt b) { return (((int64_t)a) << FP_FRAC) / b; }
constexpr fixpt float2fixpt(const float a) { return (fixpt)(a*(1 << FP_FRAC)); }
constexpr float fixpt2float(const fixpt a) { return (float)(a)/(1 << FP_FRAC); }
constexpr fixpt sqrtfixpt(const fixpt a) { return float2fixpt(sqrt(fixpt2float(a))); }

template<typename T>
constexpr fixpt int2fixpt(const T a) { return (fixpt)a << FP_FRAC; }
template<typename T>
constexpr T fixpt2int(const fixpt a) { return (T)(a >> FP_FRAC); }

#if 1
// probably use this instead?
// https://github.com/MikeLankamp/fpm/tree/master
template<uint8_t FRAC>
struct fp_t {
  static constexpr float SCALE = 1.0/ipow(2, FRAC);
  static constexpr float MIN_FIXPT = -ipow(2, 32-FRAC-1);
  static constexpr double MAX_FIXPT = ipow(2, 32-FRAC-1) - SCALE;

  int32_t value{0};
};

#endif