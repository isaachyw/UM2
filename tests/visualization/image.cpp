#include "../test_framework.hpp"
#include <um2/visualization/image.hpp>

#include "../mesh/helpers/setup_mesh.hpp"
UM2_HOSTDEV TEST_CASE(image_default_constructor)
{
  um2::Image image(10, 8);
  EXPECT_EQ(image.nx, 10);
  EXPECT_EQ(image.ny, 8);
  EXPECT_EQ(image.buffer.size(), 10 * 8);
  image.to_ppm(um2::String("image_default_constructor.ppm"));
}

template <std::floating_point T, std::integral I>
UM2_HOSTDEV TEST_CASE(render_tri_mesh)
{
  // make mesh
  um2::TriMesh<T, I> mesh;
  makeTriReferenceMesh(mesh);
  EXPECT_TRUE(!mesh.vertices.empty());
  // render mesh
  um2::Image const image(mesh);
  // save to file
  image.to_ppm(um2::String("render_tri_mesh.ppm"));
}

template <std::floating_point T, std::integral I>
UM2_HOSTDEV TEST_CASE(render_quad_mesh)
{
  // make mesh
  um2::QuadMesh<T, I> mesh;
  makeQuadReferenceMesh(mesh);
  EXPECT_TRUE(!mesh.vertices.empty());
  // render mesh
  um2::Image image(mesh);
  // save to file
  image.to_ppm(um2::String("render_quad_mesh.ppm"));
}

template <typename T, typename I>
TEST_SUITE(image_test)
{
  TEST_HOSTDEV((image_default_constructor));
  TEST_HOSTDEV((render_tri_mesh<T, I>));
  TEST_HOSTDEV((render_quad_mesh<T, I>));
}

auto main() -> int
{
  RUN_TESTS((image_test<double, int32_t>));
  return 0;
}