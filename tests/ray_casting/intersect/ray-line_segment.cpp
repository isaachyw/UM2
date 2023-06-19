#include "../../test_framework.hpp"
#include <um2/ray_casting/intersect/ray-line_segment.hpp>

template <typename T>
UM2_HOSTDEV TEST_CASE(intersect)
{
  um2::LineSegment<2, T> l(um2::Point2<T>(0, 1), um2::Point2<T>(2, -1));
  um2::Ray2<T> const ray(um2::Point2<T>(0, -1), um2::Point2<T>(1, 1).normalized());
  std::floating_point auto res = intersect(ray, l);
  EXPECT_NEAR(res, std::sqrt(2), 1e-4);

  l = um2::LineSegment<2, T>(um2::Point2<T>(1, -1), um2::Point2<T>(1, 1));
  res = intersect(ray, l);
  EXPECT_NEAR(res, std::sqrt(2), 1e-4);
}

template <typename T>
TEST_SUITE(ray2_line_segment2)
{
  TEST_HOSTDEV(intersect<T>);
}

auto main() -> int
{
  RUN_TESTS((ray2_line_segment2<float>));
  RUN_TESTS((ray2_line_segment2<double>));
  return 0;
}