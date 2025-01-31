//==============================================================================
// Findings
//==============================================================================
// On i7-12800H, sqrt<double> is 4x slower than sqrt<float>
// The time per sqrt (double) is approximately 0.6ns
// It doesn't appear that multiple threads help before 100k elements
// After about 50k elements, the Nvidia 3050 GPU is faster than the CPU

#include "../helpers.hpp"
#include <um2/stdlib/math.hpp>

constexpr Size npoints = 1 << 20;
constexpr int lo = 0;
constexpr int hi = 10000;

template <typename T>
void
sqrtCPU(benchmark::State & state)
{
  Size const n = static_cast<Size>(state.range(0));
  um2::Vector<T> x =
      makeVectorOfRandomFloats<T>(n, static_cast<T>(lo), static_cast<T>(hi));
  um2::Vector<T> sqrtx(n);
  for (auto s : state) {
    std::transform(x.begin(), x.end(), sqrtx.begin(), um2::sqrt<T>);
  }
}

#if UM2_USE_TBB
template <typename T>
void
sqrtCPUThreads(benchmark::State & state)
{
  Size const n = static_cast<Size>(state.range(0));
  um2::Vector<T> x =
      makeVectorOfRandomFloats<T>(n, static_cast<T>(lo), static_cast<T>(hi));
  um2::Vector<T> sqrtx(n);
  for (auto s : state) {
    std::transform(std::execution::par_unseq, x.begin(), x.end(), sqrtx.begin(),
                   um2::sqrt<T>);
  }
}
#endif

#if UM2_USE_CUDA
template <typename T>
static __global__ void
sqrtFloatKernel(T * x, T * sqrtx, Size const n)
{
  Size const i = threadIdx.x + blockIdx.x * blockDim.x;
  if (i < n) {
    sqrtx[i] = um2::sqrt<T>(x[i]);
  }
}

template <typename T>
void
sqrtFloatCUDA(benchmark::State & state)
{
  Size const n = static_cast<Size>(state.range(0));
  um2::Vector<T> x =
      makeVectorOfRandomFloats<T>(n, static_cast<T>(lo), static_cast<T>(hi));
  um2::Vector<T> sqrtx(n);
  T * x_d;
  T * sqrtx_d;
  transferToDevice(&x_d, x);
  transferToDevice(&sqrtx_d, sqrtx);

  constexpr uint32_t threadsPerBlock = 256;
  uint32_t const blocks =
      (static_cast<uint32_t>(n) + threadsPerBlock - 1) / threadsPerBlock;
  for (auto s : state) {
    sqrtFloatKernel<<<(blocks), threadsPerBlock>>>(x_d, sqrtx_d, n);
    cudaDeviceSynchronize();
  }

  transferFromDevice(sqrtx, sqrtx_d);
  cudaFree(x_d);
  cudaFree(sqrtx_d);
}
#endif

BENCHMARK_TEMPLATE(sqrtCPU, float)
    ->RangeMultiplier(4)
    ->Range(1024, npoints)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(sqrtCPU, double)
    ->RangeMultiplier(4)
    ->Range(1024, npoints)
    ->Unit(benchmark::kMicrosecond);

#if UM2_USE_TBB
BENCHMARK_TEMPLATE(sqrtCPUThreads, float)
    ->RangeMultiplier(4)
    ->Range(1024, npoints)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(sqrtCPUThreads, double)
    ->RangeMultiplier(4)
    ->Range(1024, npoints)
    ->Unit(benchmark::kMicrosecond);
#endif

#if UM2_USE_CUDA
BENCHMARK_TEMPLATE(sqrtFloatCUDA, float)
    ->RangeMultiplier(4)
    ->Range(65536, npoints)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(sqrtFloatCUDA, double)
    ->RangeMultiplier(4)
    ->Range(65536, npoints)
    ->Unit(benchmark::kMicrosecond);
#endif
BENCHMARK_MAIN();
