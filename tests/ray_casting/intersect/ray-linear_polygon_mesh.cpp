#include "../../test_framework.hpp"
#include <um2/ray_casting/intersect/ray-linear_polygon_mesh.hpp>

#include "../../mesh/helpers/setup_mesh.hpp"

template <std::floating_point T, std::signed_integral I>
TEST_CASE(intersect_tri_mesh)
{
  um2::TriMesh<T, I> mesh;
  makeTriReferenceMesh(mesh);
  um2::Ray2<T> const ray(um2::Point2<T>(static_cast<T>(-1), static_cast<T>(0.5)),
                         um2::Vec2<T>(1, 0));
  // cppcheck-suppress constStatement
  T * rvec = new T[8];
  int n = 8;
  intersect(ray, mesh, rvec, &n);
  EXPECT_EQ(n, 4)
  EXPECT_NEAR(rvec[0], 2.0, 1e-4)
  EXPECT_NEAR(rvec[1], 1.5, 1e-4)
  EXPECT_NEAR(rvec[2], 1.0, 1e-4)
  EXPECT_NEAR(rvec[3], 1.5, 1e-4)
  delete[] rvec;
}

template <std::floating_point T, std::signed_integral I>
TEST_CASE(intersect_quad_mesh)
{
  um2::QuadMesh<T, I> mesh;
  makeQuadReferenceMesh(mesh);
  um2::Ray2<T> ray(um2::Point2<T>(static_cast<T>(-1), static_cast<T>(0.5)),
                   um2::Vec2<T>(1, 0));
  // cppcheck-suppress constStatement
  T * rvec = new T[8];
  int n = 8;
  intersect(ray, mesh, rvec, &n);
  EXPECT_EQ(n, 4);
  EXPECT_NEAR(rvec[0], 2, 1e-4)
  EXPECT_NEAR(rvec[1], 1, 1e-4)
  EXPECT_NEAR(rvec[2], 3, 1e-4)
  EXPECT_NEAR(rvec[3], 2, 1e-4)
  delete[] rvec;
}

template <std::floating_point T, std::signed_integral I>
TEST_CASE(intersect_tri_quad_mesh)
{
  um2::TriQuadMesh<T, I> mesh;
  makeTriQuadReferenceMesh(mesh);
  um2::Ray2<T> ray(um2::Point2<T>(static_cast<T>(-1), static_cast<T>(0.5)),
                   um2::Vec2<T>(1, 0));
  // cppcheck-suppress constStatement
  T * rvec = new T[4];
  int n = 4;
  intersect(ray, mesh, rvec, &n);
  EXPECT_EQ(n, 4);
  EXPECT_NEAR(rvec[0], 2, 1e-4);
  EXPECT_NEAR(rvec[1], 1, 1e-4);
  EXPECT_NEAR(rvec[2], 2.5, 1e-4);
  EXPECT_NEAR(rvec[3], 2, 1e-4);
  delete[] rvec;
}

template <typename T, typename I>
TEST_SUITE(ray_linear_polygon_mesh)
{
  TEST_HOSTDEV((intersect_tri_mesh<T, I>));
  TEST_HOSTDEV((intersect_quad_mesh<T, I>));
  TEST_HOSTDEV((intersect_tri_quad_mesh<T, I>));
}

auto main() -> int
{
  RUN_TESTS((ray_linear_polygon_mesh<float, int32_t>));
  RUN_TESTS((ray_linear_polygon_mesh<double, int32_t>));
  return 0;
}
