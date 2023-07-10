#include "../test_framework.hpp"
#include <iostream>
#include <um2/visualization/image.hpp>
UM2_HOSTDEV TEST_CASE(image_default_constructor)
{
  um2::Image image(10, 8);
  EXPECT_EQ(image.nx, 10);
  EXPECT_EQ(image.ny, 8);
  EXPECT_EQ(image.buffer.size(), 10 * 8);
  image.to_ppm(um2::String("image_default_constructor.ppm"));
}

TEST_SUITE(image_default_constructor) { TEST(image_default_constructor); }

auto main() -> int
{
  RUN_TESTS(image_default_constructor);
  return 0;
}