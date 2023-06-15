#include "../test_framework.hpp"
#include <um2/mesh/mesh_type.hpp>

UM2_HOSTDEV TEST_CASE(intToMeshtype)
{
  using um2::mesh_type, um2::intToMeshtype;
  EXPECT_EQ(intToMeshtype(1), mesh_type::tri)
  EXPECT_EQ(intToMeshtype(2), mesh_type::quad)
  EXPECT_EQ(intToMeshtype(3), mesh_type::tri_quad)
  EXPECT_EQ(intToMeshtype(4), mesh_type::quadratic_tri)
  EXPECT_EQ(intToMeshtype(5), mesh_type::quadratic_quad)
  EXPECT_EQ(intToMeshtype(6), mesh_type::quadratic_tri_quad)
}

UM2_HOSTDEV TEST_CASE(meshtypeToString)
{
  using um2::meshtypeToString;
  EXPECT_EQ(meshtypeToString(0), "ERROR")
  EXPECT_EQ(meshtypeToString(1), "TRI")
  EXPECT_EQ(meshtypeToString(2), "QUAD")
  EXPECT_EQ(meshtypeToString(3), "TRI_QUAD")
  EXPECT_EQ(meshtypeToString(4), "QUADRATIC_TRI")
  EXPECT_EQ(meshtypeToString(5), "QUADRATIC_QUAD")
  EXPECT_EQ(meshtypeToString(6), "QUADRATIC_TRI_QUAD")
}

UM2_HOSTDEV TEST_CASE(meshtypeIs2d)
{
  using um2::meshtypeIs2d;
  EXPECT_EQ(meshtypeIs2d(0), false)
  EXPECT_EQ(meshtypeIs2d(1), true)
  EXPECT_EQ(meshtypeIs2d(2), true)
  EXPECT_EQ(meshtypeIs2d(3), true)
  EXPECT_EQ(meshtypeIs2d(4), true)
  EXPECT_EQ(meshtypeIs2d(5), true)
  EXPECT_EQ(meshtypeIs2d(6), true)
}

TEST_SUITE(mesh_type)
{
  TEST_HOSTDEV(intToMeshtype)
  TEST_HOSTDEV(meshtypeToString)
  TEST_HOSTDEV(meshtypeIs2d)
}

auto main() -> int
{
  RUN_TESTS(mesh_type)
  return 0;
}