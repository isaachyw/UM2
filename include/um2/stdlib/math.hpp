#pragma once

#include <um2/config.hpp>

#include <cmath>
#include <concepts>

// Contains:
//  abs
//  atan
//  atanh
//  cbrt
//  ceil
//  cos
//  exp
//  floor
//  sin
//  sqrt

namespace um2
{

template <std::floating_point T>
constexpr T pi = static_cast<T>(3.14159265358979323846);

template <std::floating_point T>
constexpr T pi_2 = static_cast<T>(1.57079632679489661923);

//==============================================================================
// abs
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
abs(T x) noexcept -> T
{
  return std::abs(x);
}

#else

PURE DEVICE constexpr auto
abs(float x) noexcept -> float
{
  return ::fabsf(x);
}

PURE DEVICE constexpr auto
abs(double x) noexcept -> double
{
  return ::fabs(x);
}

#endif

//==============================================================================
// atan
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
atan(T x) noexcept -> T
{
  return std::atan(x);
}

#else

template <typename T>
PURE HOST constexpr auto
atan(T x) noexcept -> T
{
  static_assert(always_false<T>, "atan is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constexpr auto
atan(float x) noexcept -> float
{
  return ::atanf(x);
}

template <>
PURE DEVICE constexpr auto
atan(double x) noexcept -> double
{
  return ::atan(x);
}

#endif

//==============================================================================
// atanh
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
atanh(T x) noexcept -> T
{
  return std::atanh(x);
}

#else

template <typename T>
PURE HOST constexpr auto
atanh(T x) noexcept -> T
{
  static_assert(always_false<T>, "atanh is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constexpr auto
atanh(float x) noexcept -> float
{
  return ::atanhf(x);
}

template <>
PURE DEVICE constexpr auto
atanh(double x) noexcept -> double
{
  return ::atanh(x);
}

#endif

//==============================================================================
// cbrt
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
cbrt(T x) noexcept -> T
{
  return std::cbrt(x);
}

#else

template <typename T>
PURE HOST constexpr auto
cbrt(T x) noexcept -> T
{
  static_assert(always_false<T>, "cbrt is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constexpr auto
cbrt(float x) noexcept -> float
{
  return ::cbrtf(x);
}

template <>
PURE DEVICE constexpr auto
cbrt(double x) noexcept -> double
{
  return ::cbrt(x);
}

#endif

//==============================================================================
// ceil
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
ceil(T x) noexcept -> T
{
  return std::ceil(x);
}

#else

PURE DEVICE constexpr auto
ceil(float x) noexcept -> float
{
  return ::ceilf(x);
}

PURE DEVICE constexpr auto
ceil(double x) noexcept -> double
{
  return ::ceil(x);
}

#endif

//==============================================================================
// cos
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
cos(T x) noexcept -> T
{
  return std::cos(x);
}

#else

template <typename T>
PURE HOST constcosr auto
cos(T x) noexcept -> T
{
  static_assert(always_false<T>, "cos is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constcosr auto
cos(float x) noexcept -> float
{
  return ::cosf(x);
}

template <>
PURE DEVICE constcosr auto
cos(double x) noexcept -> double
{
  return ::cos(x);
}

#endif

//==============================================================================
// exp
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
exp(T x) noexcept -> T
{
  return std::exp(x);
}

#else

template <typename T>
PURE HOST constexpr auto
exp(T x) noexcept -> T
{
  static_assert(always_false<T>, "exp is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constexpr auto
exp(float x) noexcept -> float
{
  return ::expf(x);
}

template <>
PURE DEVICE constexpr auto
exp(double x) noexcept -> double
{
  return ::exp(x);
}

#endif

//==============================================================================
// floor
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
floor(T x) noexcept -> T
{
  return std::floor(x);
}

#else

PURE DEVICE constexpr auto
floor(float x) noexcept -> float
{
  return ::floorf(x);
}

PURE DEVICE constexpr auto
floor(double x) noexcept -> double
{
  return ::floor(x);
}

#endif

//==============================================================================
// sin
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
sin(T x) noexcept -> T
{
  return std::sin(x);
}

#else

PURE DEVICE constexpr auto
sin(float x) noexcept -> float
{
  return ::sinf(x);
}

PURE DEVICE constexpr auto
sin(double x) noexcept -> double
{
  return ::sin(x);
}

#endif

//==============================================================================
// sqrt
//==============================================================================

#ifndef __CUDA_ARCH__

template <typename T>
PURE HOST constexpr auto
sqrt(T x) noexcept -> T
{
  return std::sqrt(x);
}

#else

template <typename T>
PURE HOST constexpr auto
sqrt(T x) noexcept -> T
{
  static_assert(always_false<T>, "sqrt is not implemented for this type");
  return T();
}

template <>
PURE DEVICE constexpr auto
sqrt(float x) noexcept -> float
{
  return ::sqrtf(x);
}

template <>
PURE DEVICE constexpr auto
sqrt(double x) noexcept -> double
{
  return ::sqrt(x);
}

#endif

} // namespace um2
