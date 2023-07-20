#pragma once

#include <cassert> // assert
#include <cstdint> // int32_t, int64_t

// OpenMP
#define UM2_ENABLE_OPENMP 1

// CUDA
#define UM2_ENABLE_CUDA 0
#undef HOST
#undef DEVICE
#undef HOSTDEV
#undef GLOBAL
#if UM2_ENABLE_CUDA
#  include <cuda_runtime.h>
#  define HOST    __host__
#  define DEVICE  __device__
#  define HOSTDEV __host__ __device__
#  define GLOBAL  __global__
#else
#  define HOST
#  define DEVICE
#  define HOSTDEV
#  define GLOBAL
#endif

// Visualization
#define UM2_ENABLE_VIS 0

// Attributes
// -----------------------------------------------------------------------------
// These are supported by GCC and Clang, but cause issues when using nvcc
#undef PURE
#undef CONST
#undef HOT
#undef HIDDEN
#if UM2_ENABLE_CUDA
#  define PURE
#  define CONST
#  define HOT
#else
#  define PURE  [[gnu::pure]]
#  define CONST [[gnu::const]]
#  define HOT   [[gnu::hot]]
#endif
#define HIDDEN __attribute__((visibility("hidden")))

// Typedefs
// -----------------------------------------------------------------------------
#define UM2_ENABLE_INT64 0
#if UM2_ENABLE_INT64
using Size = int64_t;
consteval auto sizeMax() -> Size { return 0x7FFFFFFFFFFFFFFF; }
#else
using Size = int32_t;
consteval auto sizeMax() -> Size { return 0x7FFFFFFF; }
#endif
