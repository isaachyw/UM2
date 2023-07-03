#include "../test_framework.hpp"
#include <um2/mesh/regular_grid.hpp>

template <len_t D, typename T>
UM2_HOSTDEV static constexpr auto makeGrid() -> um2::RegularGrid<D, T>
{
  static_assert(1 <= D && D <= 3, "D must be in [1, 3]");
  um2::RegularGrid<D, T> grid;
  if constexpr (D >= 1) {
    grid.minima.data()[0] = 1;
    grid.spacing.data()[0] = 1;
    grid.num_cells.data()[0] = 1;
  }
  if constexpr (D >= 2) {
    grid.minima.data()[1] = 2;
    grid.spacing.data()[1] = 2;
    grid.num_cells.data()[1] = 2;
  }
  if constexpr (D >= 3) {
    grid.minima.data()[2] = 3;
    grid.spacing.data()[2] = 3;
    grid.num_cells.data()[2] = 3;
  }
  return grid;
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(constructor)
{
  um2::RegularGrid<D, T> grid_ref = makeGrid<D, T>();
  um2::RegularGrid<D, T> grid(grid_ref.minima, grid_ref.spacing, grid_ref.num_cells);
  if constexpr (D >= 1) {
    EXPECT_NEAR(grid.minima[0], grid_ref.minima[0], 1e-6);
    EXPECT_NEAR(grid.spacing[0], grid_ref.spacing[0], 1e-6);
    EXPECT_EQ(grid.num_cells[0], grid_ref.num_cells[0]);
  }
  if constexpr (D >= 2) {
    EXPECT_NEAR(grid.minima[1], grid_ref.minima[1], 1e-6);
    EXPECT_NEAR(grid.spacing[1], grid_ref.spacing[1], 1e-6);
    EXPECT_EQ(grid.num_cells[1], grid_ref.num_cells[1]);
  }
  if constexpr (D >= 3) {
    EXPECT_NEAR(grid.minima[2], grid_ref.minima[2], 1e-6);
    EXPECT_NEAR(grid.spacing[2], grid_ref.spacing[2], 1e-6);
    EXPECT_EQ(grid.num_cells[2], grid_ref.num_cells[2]);
  }
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(getRangeInGrid)
{
  um2::RegularGrid<2, T> grid = makeGrid<2, T>();
  um2::AABox<2, T> box{
      um2::Point<2, T>{5.75, 4.5},
      um2::Point<2, T>{6.75, 6.1},
  };
  um2::Vec<4, len_t> range = grid.get_range_in_grid(box);
  EXPECT_TRUE(range[0] != 0);
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(accessors)
{
  um2::RegularGrid<D, T> grid = makeGrid<D, T>();
  um2::Vec<D, len_t> ncells = numCells(grid);
  if constexpr (D >= 1) {
    auto const nx = grid.num_cells[0];
    T const x_min = grid.minima[0];
    T const x_max = x_min + grid.spacing[0] * static_cast<T>(nx);
    EXPECT_NEAR(xMin(grid), x_min, 1e-6);
    EXPECT_NEAR(xMax(grid), x_max, 1e-6);
    EXPECT_EQ(numXcells(grid), nx);
    EXPECT_EQ(ncells[0], nx);
    EXPECT_NEAR(width(grid), x_max - x_min, 1e-6);
  }
  if constexpr (D >= 2) {
    auto const ny = grid.num_cells[1];
    T const ymin = grid.minima[1];
    T const ymax = ymin + grid.spacing[1] * static_cast<T>(ny);
    EXPECT_NEAR(yMin(grid), ymin, 1e-6);
    EXPECT_NEAR(yMax(grid), ymax, 1e-6);
    EXPECT_EQ(numYcells(grid), ny);
    EXPECT_EQ(ncells[1], ny);
    EXPECT_NEAR(height(grid), ymax - ymin, 1e-6);
  }
  if constexpr (D >= 3) {
    auto const nz = grid.num_cells[2];
    T const zmin = grid.minima[2];
    T const zmax = zmin + grid.spacing[2] * static_cast<T>(nz);
    EXPECT_NEAR(zMin(grid), zmin, 1e-6);
    EXPECT_NEAR(zMax(grid), zmax, 1e-6);
    EXPECT_EQ(numZcells(grid), nz);
    EXPECT_EQ(ncells[2], nz);
    EXPECT_NEAR(depth(grid), zmax - zmin, 1e-6);
  }
}

template <len_t D, typename T>
UM2_HOSTDEV TEST_CASE(bounding_box)
{
  um2::RegularGrid<D, T> const grid = makeGrid<D, T>();
  um2::AABox<D, T> box = boundingBox(grid);
  if constexpr (D >= 1) {
    EXPECT_NEAR(box.minima[0], xMin(grid), 1e-6);
    EXPECT_NEAR(box.maxima[0], xMax(grid), 1e-6);
  }
  if constexpr (D >= 2) {
    EXPECT_NEAR(box.minima[1], yMin(grid), 1e-6);
    EXPECT_NEAR(box.maxima[1], yMax(grid), 1e-6);
  }
  if constexpr (D >= 3) {
    EXPECT_NEAR(box.minima[2], zMin(grid), 1e-6);
    EXPECT_NEAR(box.maxima[2], zMax(grid), 1e-6);
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
  um2::Point2<T> minima = {1, -1};
  um2::Vec2<T> spacing = {half, forth};
  um2::Vec2<len_t> num_cells = {4, 8};
  um2::RegularGrid2<T> grid(minima, spacing, num_cells);
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

#if UM2_ENABLE_CUDA
template <len_t D, typename T>
MAKE_CUDA_KERNEL(constructor, D, T)

template <len_t D, typename T>
MAKE_CUDA_KERNEL(accessors, D, T)

template <len_t D, typename T>
MAKE_CUDA_KERNEL(bounding_box, D, T)

template <len_t D, typename T>
MAKE_CUDA_KERNEL(get_box, D, T)

#endif

template <len_t D, typename T>
TEST_SUITE(regular_grid)
{
  TEST_HOSTDEV((constructor<D, T>));
  TEST_HOSTDEV((accessors<D, T>));
  TEST_HOSTDEV((bounding_box<D, T>));
  if constexpr (D == 2) {
    TEST_HOSTDEV((getBox<T>));
    TEST_HOSTDEV((getRangeInGrid<D, T>));
  }
}

auto main() -> int
{
  RUN_TESTS((regular_grid<1, float>));
  RUN_TESTS((regular_grid<1, double>));
  RUN_TESTS((regular_grid<2, float>));
  RUN_TESTS((regular_grid<2, double>));
  RUN_TESTS((regular_grid<3, float>));
  RUN_TESTS((regular_grid<3, double>));
  return 0;
}