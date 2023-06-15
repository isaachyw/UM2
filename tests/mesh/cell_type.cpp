#include "../test_framework.hpp"
#include <um2/mesh/cell_type.hpp>

UM2_HOSTDEV TEST_CASE(vtk2xdmf)
{
  using um2::vtk_cell_type, um2::xdmf_cell_type, um2::vtk2xdmf;
  vtk_cell_type const tri = vtk_cell_type::triangle;
  vtk_cell_type const quad = vtk_cell_type::quad;
  vtk_cell_type const tri6 = vtk_cell_type::quadratic_triangle;
  vtk_cell_type const quad8 = vtk_cell_type::quadratic_quad;
  EXPECT_EQ(vtk2xdmf(tri), xdmf_cell_type::triangle)
  EXPECT_EQ(vtk2xdmf(quad), xdmf_cell_type::quad)
  EXPECT_EQ(vtk2xdmf(tri6), xdmf_cell_type::quadratic_triangle)
  EXPECT_EQ(vtk2xdmf(quad8), xdmf_cell_type::quadratic_quad)
  auto u8_tri = static_cast<int8_t>(vtk_cell_type::triangle);
  auto u8_quad = static_cast<int8_t>(vtk_cell_type::quad);
  auto u8_tri6 = static_cast<int8_t>(vtk_cell_type::quadratic_triangle);
  auto u8_quad8 = static_cast<int8_t>(vtk_cell_type::quadratic_quad);
  EXPECT_EQ(vtk2xdmf(u8_tri), static_cast<int8_t>(xdmf_cell_type::triangle))
  EXPECT_EQ(vtk2xdmf(u8_quad), static_cast<int8_t>(xdmf_cell_type::quad))
  EXPECT_EQ(vtk2xdmf(u8_tri6), static_cast<int8_t>(xdmf_cell_type::quadratic_triangle))
  EXPECT_EQ(vtk2xdmf(u8_quad8), static_cast<int8_t>(xdmf_cell_type::quadratic_quad))
}

UM2_HOSTDEV TEST_CASE(abaqus2xdmf)
{
  using um2::abaqus_cell_type, um2::xdmf_cell_type, um2::abaqus2xdmf;
  abaqus_cell_type const tri = abaqus_cell_type::cp_s3;
  abaqus_cell_type const quad = abaqus_cell_type::cp_s4;
  abaqus_cell_type const tri6 = abaqus_cell_type::cp_s6;
  abaqus_cell_type const quad8 = abaqus_cell_type::cp_s8;
  EXPECT_EQ(abaqus2xdmf(tri), xdmf_cell_type::triangle)
  EXPECT_EQ(abaqus2xdmf(quad), xdmf_cell_type::quad)
  EXPECT_EQ(abaqus2xdmf(tri6), xdmf_cell_type::quadratic_triangle)
  EXPECT_EQ(abaqus2xdmf(quad8), xdmf_cell_type::quadratic_quad)
  auto u8_tri = static_cast<int8_t>(abaqus_cell_type::cp_s3);
  auto u8_quad = static_cast<int8_t>(abaqus_cell_type::cp_s4);
  auto u8_tri6 = static_cast<int8_t>(abaqus_cell_type::cp_s6);
  auto u8_quad8 = static_cast<int8_t>(abaqus_cell_type::cp_s8);
  EXPECT_EQ(abaqus2xdmf(u8_tri), static_cast<int8_t>(xdmf_cell_type::triangle))
  EXPECT_EQ(abaqus2xdmf(u8_quad), static_cast<int8_t>(xdmf_cell_type::quad))
  EXPECT_EQ(abaqus2xdmf(u8_tri6), static_cast<int8_t>(xdmf_cell_type::quadratic_triangle))
  EXPECT_EQ(abaqus2xdmf(u8_quad8), static_cast<int8_t>(xdmf_cell_type::quadratic_quad))
}

UM2_HOSTDEV TEST_CASE(xdmf2vtk)
{
  using um2::xdmf_cell_type, um2::vtk_cell_type, um2::xdmf2vtk;
  xdmf_cell_type const tri = xdmf_cell_type::triangle;
  xdmf_cell_type const quad = xdmf_cell_type::quad;
  xdmf_cell_type const tri6 = xdmf_cell_type::quadratic_triangle;
  xdmf_cell_type const quad8 = xdmf_cell_type::quadratic_quad;
  EXPECT_EQ(xdmf2vtk(tri), vtk_cell_type::triangle)
  EXPECT_EQ(xdmf2vtk(quad), vtk_cell_type::quad)
  EXPECT_EQ(xdmf2vtk(tri6), vtk_cell_type::quadratic_triangle)
  EXPECT_EQ(xdmf2vtk(quad8), vtk_cell_type::quadratic_quad)
  auto u8_tri = static_cast<int8_t>(xdmf_cell_type::triangle);
  auto u8_quad = static_cast<int8_t>(xdmf_cell_type::quad);
  auto u8_tri6 = static_cast<int8_t>(xdmf_cell_type::quadratic_triangle);
  auto u8_quad8 = static_cast<int8_t>(xdmf_cell_type::quadratic_quad);
  EXPECT_EQ(xdmf2vtk(u8_tri), static_cast<int8_t>(vtk_cell_type::triangle))
  EXPECT_EQ(xdmf2vtk(u8_quad), static_cast<int8_t>(vtk_cell_type::quad))
  EXPECT_EQ(xdmf2vtk(u8_tri6), static_cast<int8_t>(vtk_cell_type::quadratic_triangle))
  EXPECT_EQ(xdmf2vtk(u8_quad8), static_cast<int8_t>(vtk_cell_type::quadratic_quad))
}

UM2_HOSTDEV TEST_CASE(isLinear)
{
  using um2::vtk_cell_type, um2::abaqus_cell_type;
  EXPECT_TRUE(isLinear(vtk_cell_type::triangle))
  EXPECT_TRUE(isLinear(vtk_cell_type::quad))
  EXPECT_TRUE(!isLinear(vtk_cell_type::quadratic_triangle))
  EXPECT_TRUE(!isLinear(vtk_cell_type::quadratic_quad))

  EXPECT_TRUE(isLinear(abaqus_cell_type::cp_s3))
  EXPECT_TRUE(isLinear(abaqus_cell_type::cp_s4))
  EXPECT_TRUE(!isLinear(abaqus_cell_type::cp_s6))
  EXPECT_TRUE(!isLinear(abaqus_cell_type::cp_s8))
}

UM2_HOSTDEV TEST_CASE(points_in_cell)
{
  using um2::xdmf_cell_type, um2::pointsInXdmfCell;
  EXPECT_EQ(pointsInXdmfCell(static_cast<int8_t>(xdmf_cell_type::triangle)), 3)
  EXPECT_EQ(pointsInXdmfCell(static_cast<int8_t>(xdmf_cell_type::quad)), 4)
  EXPECT_EQ(pointsInXdmfCell(static_cast<int8_t>(xdmf_cell_type::quadratic_triangle)), 6)
  EXPECT_EQ(pointsInXdmfCell(static_cast<int8_t>(xdmf_cell_type::quadratic_quad)), 8)
}

TEST_SUITE(cell_type)
{
  TEST_HOSTDEV(vtk2xdmf)
  TEST_HOSTDEV(abaqus2xdmf)
  TEST_HOSTDEV(xdmf2vtk)
  TEST_HOSTDEV(isLinear)
  TEST_HOSTDEV(points_in_cell)
}

auto main() -> int
{
  RUN_TESTS(cell_type)
  return 0;
}