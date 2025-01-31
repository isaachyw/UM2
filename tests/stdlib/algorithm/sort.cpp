#include <um2/stdlib/algorithm.hpp>

#include "../../test_macros.hpp"

HOSTDEV
TEST_CASE(insertionSort)
{
  int a[9] = {3, 7, 4, 9, 5, 2, 6, 1, 8};
  um2::insertionSort(&a[0], &a[0] + 9);
  for (int i = 1; i < 10; ++i) {
    ASSERT(a[i - 1] == i);
  }
}
MAKE_CUDA_KERNEL(insertionSort);

TEST_SUITE(sort) { TEST_HOSTDEV(insertionSort, 1, 1); }

auto
main() -> int
{
  RUN_SUITE(sort);
  return 0;
}
