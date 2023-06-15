#include "../test_framework.hpp"
#include <um2/mesh/rectilinear_grid.hpp>

template <len_t D, typename T>
UM2_HOSTDEV static constexpr auto makeGrid() -> um2::RectilinearGrid<D, T>
{
  um2::RectilinearGrid<D, T> grid;
  if constexpr (D >= 1) {
    grid.divs[0] = {0, 1};
  }
  if constexpr (D >= 2) {
    grid.divs[1] = {0, 1, 2};
  }
  if constexpr (D >= 3) {
    grid.divs[2] = {0, 1, 2, 3};
  }
  return grid;
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(clear)
{
  um2::RectilinearGrid<D, T> grid = makeGrid<D, T>();
  grid.clear();
  for (len_t i = 0; i < D; ++i) {
    EXPECT_TRUE(grid.divs[i].empty());
  }
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(accessors)
{
  um2::RectilinearGrid<D, T> grid = makeGrid<D, T>();
  um2::Vec<D, len_t> ncells = numCells(grid);
  if constexpr (D >= 1) {
    auto const nx = 1;
    EXPECT_NEAR(xMin(grid), grid.divs[0][0], 1e-6);
    EXPECT_NEAR(xMax(grid), grid.divs[0][nx], 1e-6);
    EXPECT_EQ(numXcells(grid), nx);
    EXPECT_EQ(ncells[0], nx);
    EXPECT_NEAR(width(grid), grid.divs[0][nx] - grid.divs[0][0], 1e-6);
  }
  if constexpr (D >= 2) {
    auto const ny = 2;
    EXPECT_NEAR(yMin(grid), grid.divs[1][0], 1e-6);
    EXPECT_NEAR(yMax(grid), grid.divs[1][ny], 1e-6);
    EXPECT_EQ(numYcells(grid), ny);
    EXPECT_EQ(ncells[1], ny);
    EXPECT_NEAR(height(grid), grid.divs[1][ny] - grid.divs[1][0], 1e-6);
  }
  if constexpr (D >= 3) {
    auto const nz = 3;
    EXPECT_NEAR(zMin(grid), grid.divs[2][0], 1e-6);
    EXPECT_NEAR(zMax(grid), grid.divs[2][nz], 1e-6);
    EXPECT_EQ(numZcells(grid), nz);
    EXPECT_EQ(ncells[2], nz);
    EXPECT_NEAR(depth(grid), grid.divs[2][nz] - grid.divs[2][0], 1e-6);
  }
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(bounding_box)
{
  um2::RectilinearGrid<D, T> grid = makeGrid<D, T>();
  um2::AABox<D, T> box = boundingBox(grid);
  if constexpr (D >= 1) {
    EXPECT_NEAR(box.minima[0], grid.divs[0][0], 1e-6);
    EXPECT_NEAR(box.maxima[0], grid.divs[0][1], 1e-6);
  }
  if constexpr (D >= 2) {
    EXPECT_NEAR(box.minima[1], grid.divs[1][0], 1e-6);
    EXPECT_NEAR(box.maxima[1], grid.divs[1][2], 1e-6);
  }
  if constexpr (D >= 3) {
    EXPECT_NEAR(box.minima[2], grid.divs[2][0], 1e-6);
    EXPECT_NEAR(box.maxima[2], grid.divs[2][3], 1e-6);
  }
}

template <typename T>
UM2_HOSTDEV TEST_CASE(getBox)
{
  // Declare some variables to avoid a bunch of static casts.
  T const three = static_cast<T>(3);
  T const two = static_cast<T>(2);
  T const one = static_cast<T>(1);
  T const half = static_cast<T>(0.5);
  T const forth = static_cast<T>(0.25);
  um2::RectilinearGrid2<T> grid;
  grid.divs[0] = {1.0, 1.5, 2.0, 2.5, 3.0};
  grid.divs[1] = {-1.0, -0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75, 1.0};
  um2::AABox2<T> box = grid.getBox(0, 0);
  um2::AABox2<T> box_ref = {
      {         1,             -1},
      {one + half, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = grid.getBox(1, 0);
  //{ { 1.5, -1.0 }, { 2.0, -0.75 } };
  box_ref = {
      {one + half,           -one},
      {       two, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = grid.getBox(3, 0);
  // box_ref = { { 2.5, -1.0 }, { 3.0, -0.75 } };
  box_ref = {
      {two + half,           -one},
      {     three, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = grid.getBox(0, 1);
  // box_ref = { { 1.0, -0.75 }, { 1.5, -0.5 } };
  box_ref = {
      {       one, -three * forth},
      {one + half,          -half}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = grid.getBox(0, 7);
  // box_ref = { { 1.0, 0.75 }, { 1.5, 1.0 } };
  box_ref = {
      {       one, three * forth},
      {one + half,           one}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = grid.getBox(3, 7);
  // box_ref = { { 2.5, 0.75 }, { 3.0, 1.0 } };
  box_ref = {
      {two + half, three * forth},
      {     three,           one}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
}

template <typename T>
UM2_HOSTDEV TEST_CASE(aabb2_constructor)
{
  um2::AABox2<T> b00(um2::Point2<T>(0, 0), um2::Point2<T>(1, 1));
  um2::AABox2<T> b10(um2::Point2<T>(1, 0), um2::Point2<T>(2, 1));
  um2::AABox2<T> b01(um2::Point2<T>(0, 1), um2::Point2<T>(1, 2));
  um2::AABox2<T> b11(um2::Point2<T>(1, 1), um2::Point2<T>(2, 2));
  um2::AABox2<T> b02(um2::Point2<T>(0, 2), um2::Point2<T>(1, 3));
  um2::AABox2<T> b12(um2::Point2<T>(1, 2), um2::Point2<T>(2, 3));
  um2::Vector<um2::AABox2<T>> boxes = {b00, b10, b01, b11, b02, b12};
  um2::RectilinearGrid2<T> grid(boxes);

  EXPECT_EQ(grid.divs[0].size(), 3);
  T const xref[3] = {0, 1, 2};
  for (len_t i = 0; i < 3; ++i) {
    EXPECT_NEAR(grid.divs[0][i], xref[i], 1e-6);
  }
  EXPECT_EQ(grid.divs[1].size(), 4);
  T const yref[4] = {0, 1, 2, 3};
  for (len_t i = 0; i < 4; ++i) {
    EXPECT_NEAR(grid.divs[1][i], yref[i], 1e-6);
  }

  um2::RectilinearGrid2<T> grid2(b01);
  EXPECT_EQ(grid2.divs[0].size(), 2);
  EXPECT_EQ(grid2.divs[1].size(), 2);
  EXPECT_NEAR(grid2.divs[0][0], 0, 1e-6);
  EXPECT_NEAR(grid2.divs[0][1], 1, 1e-6);
  EXPECT_NEAR(grid2.divs[1][0], 1, 1e-6);
  EXPECT_NEAR(grid2.divs[1][1], 2, 1e-6);
}

template <typename T>
TEST_CASE(id_array_constructor)
{
  std::vector<std::vector<int>> ids = {
      {0, 1, 2, 0},
      {0, 2, 0, 2},
      {0, 1, 0, 1},
  };
  std::vector<um2::Vec2<T>> dxdy = {
      {2, 1},
      {2, 1},
      {2, 1},
      {2, 1},
  };
  um2::RectilinearGrid2<T> grid(dxdy, ids);

  EXPECT_EQ(grid.divs[0].size(), 5);
  T const xref[5] = {0, 2, 4, 6, 8};
  for (len_t i = 0; i < 5; ++i) {
    EXPECT_NEAR(grid.divs[0][i], xref[i], 1e-6);
  }
  EXPECT_EQ(grid.divs[1].size(), 4);
  T const yref[4] = {0, 1, 2, 3};
  for (len_t i = 0; i < 4; ++i) {
    EXPECT_NEAR(grid.divs[1][i], yref[i], 1e-6);
  }
}

#if UM2_ENABLE_CUDA
template <len_t D, typename T>
MAKE_CUDA_KERNEL(clear, D, T)

template <len_t D, typename T>
MAKE_CUDA_KERNEL(accessors, D, T)

template <len_t D, typename T>
MAKE_CUDA_KERNEL(bounding_box, D, T)

template <typename T>
MAKE_CUDA_KERNEL(getBox, T)

template <typename T>
MAKE_CUDA_KERNEL(aabb2_constructor, T)
#endif

template <len_t D, typename T>
TEST_SUITE(rectilinear_grid)
{
  TEST_HOSTDEV((clear<D, T>));
  TEST_HOSTDEV((accessors<D, T>));
  TEST_HOSTDEV((bounding_box<D, T>));
  if constexpr (D == 2) {
    TEST_HOSTDEV((getBox<T>));
    TEST_HOSTDEV((aabb2_constructor<T>));
    TEST_HOSTDEV((id_array_constructor<T>));
  }
}

auto main() -> int
{
  RUN_TESTS((rectilinear_grid<1, float>));
  RUN_TESTS((rectilinear_grid<1, double>));
  RUN_TESTS((rectilinear_grid<2, float>));
  RUN_TESTS((rectilinear_grid<2, double>));
  RUN_TESTS((rectilinear_grid<3, float>));
  RUN_TESTS((rectilinear_grid<3, double>));
  return 0;
}