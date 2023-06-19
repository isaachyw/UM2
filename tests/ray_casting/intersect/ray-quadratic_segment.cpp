#include "../../test_framework.hpp"
#include <um2/ray_casting/intersect/ray-quadratic_segment.hpp>

template <typename T>
UM2_HOSTDEV TEST_CASE(intersect)
{
  um2::QuadraticSegment<2, T> q(um2::Point2<T>(0, 0), um2::Point2<T>(2, 0),
                                um2::Point2<T>(1, 1));
  um2::Ray2<T> ray(um2::Point2<T>(1, -1), um2::Point2<T>(0, 1));
  auto res = intersect(ray, q);
  EXPECT_NEAR(res[0], 2, 1e-4);
  EXPECT_NEAR(res[1], um2::infiniteDistance<T>(), 1e-4);

  ray = {um2::Point2<T>(static_cast<T>(0.0), static_cast<T>(0.75)), um2::Point2<T>(1, 0)};
  res = intersect(ray, q);
  EXPECT_NEAR(res[0], 0.5, 1e-4);
  EXPECT_NEAR(res[1], 1.5, 1e-4);
}

template <typename T>
TEST_SUITE(ray2_quadratic_segment2)
{
  TEST_HOSTDEV((intersect<T>));
}

auto main() -> int
{
  RUN_TESTS((ray2_quadratic_segment2<float>));
  RUN_TESTS((ray2_quadratic_segment2<double>));
  return 0;
}