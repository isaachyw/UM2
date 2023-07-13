#include "../test_framework.hpp"
#include <um2/mesh/face_vertex_mesh.hpp>
#include <um2/mesh/regular_partition.hpp>

#include "../mesh/helpers/setup_mesh.hpp"
template <len_t D, typename T, typename P>
UM2_HOSTDEV static constexpr auto makePart() -> um2::RegularPartition<D, T, P>
{
  static_assert(1 <= D && D <= 3, "D must be in [1, 3]");
  um2::RegularGrid<D, T> grid;
  if constexpr (D >= 1) {
    grid.minima[0] = 1;
    grid.spacing[0] = 1;
    grid.num_cells[0] = 1;
  }
  if constexpr (D >= 2) {
    grid.minima[1] = 2;
    grid.spacing[1] = 2;
    grid.num_cells[1] = 2;
  }
  if constexpr (D >= 3) {
    grid.minima[2] = 3;
    grid.spacing[2] = 3;
    grid.num_cells[2] = 3;
  }
  um2::RegularPartition<D, T, P> part;
  part.grid = grid;
  if constexpr (D == 1) {
    part.children = {1};
  } else if constexpr (D == 2) {
    part.children = {1, 2};
  } else if constexpr (D >= 3) {
    part.children = {1, 2, 3, 4, 5, 6};
  }
  return part;
}

template <len_t D, typename T, typename P>
UM2_HOSTDEV static constexpr auto makePartEmpty() -> um2::RegularPartition<D, T, P>
{
  // make a partition with no children
  static_assert(1 <= D && D <= 3, "D must be in [1, 3]");
  um2::RegularGrid<D, T> grid;
  if constexpr (D >= 1) {
    grid.minima[0] = -1;
    grid.spacing[0] = 1.5;
    grid.num_cells[0] = 2;
  }
  if constexpr (D >= 2) {
    grid.minima[1] = -1;
    grid.spacing[1] = 1.5;
    grid.num_cells[1] = 2;
  }
  if constexpr (D >= 3) {
    grid.minima[2] = 3;
    grid.spacing[2] = 3;
    grid.num_cells[2] = 3;
  }
  um2::RegularPartition<D, T, P> part;
  part.grid = grid;
  return part;
}

template <len_t D, typename T, typename P>
UM2_HOSTDEV TEST_CASE(accessors)
{
  um2::RegularPartition<D, T, P> part = makePart<D, T, P>();
  um2::Vec<D, len_t> ncells = numCells(part);
  if constexpr (D >= 1) {
    auto const nx = part.grid.num_cells[0];
    T const xmin = part.grid.minima[0];
    T const xmax = xmin + part.grid.spacing[0] * static_cast<T>(nx);
    EXPECT_NEAR(xMin(part), xmin, 1e-6);
    EXPECT_NEAR(xMax(part), xmax, 1e-6);
    EXPECT_EQ(numXcells(part), nx);
    EXPECT_EQ(ncells[0], nx);
    EXPECT_NEAR(width(part), xmax - xmin, 1e-6)
  }
  if constexpr (D >= 2) {
    auto const ny = part.grid.num_cells[1];
    T const ymin = part.grid.minima[1];
    T const ymax = ymin + part.grid.spacing[1] * static_cast<T>(ny);
    EXPECT_NEAR(yMin(part), ymin, 1e-6);
    EXPECT_NEAR(yMax(part), ymax, 1e-6);
    EXPECT_EQ(numYcells(part), ny);
    EXPECT_EQ(ncells[1], ny);
    EXPECT_NEAR(height(part), ymax - ymin, 1e-6);
  }
  if constexpr (D >= 3) {
    auto const nz = part.grid.num_cells[2];
    T const zmin = part.grid.minima[2];
    T const zmax = zmin + part.grid.spacing[2] * static_cast<T>(nz);
    EXPECT_NEAR(zMin(part), zmin, 1e-6);
    EXPECT_NEAR(zMax(part), zmax, 1e-6);
    EXPECT_EQ(numZcells(part), nz);
    EXPECT_EQ(ncells[2], nz);
    EXPECT_NEAR(depth(part), zmax - zmin, 1e-6);
  }
}

template <len_t D, typename T, typename P>
UM2_HOSTDEV TEST_CASE(bounding_box)
{
  um2::RegularPartition<D, T, P> part = makePart<D, T, P>();
  um2::AABox<D, T> box = boundingBox(part);
  if constexpr (D >= 1) {
    EXPECT_NEAR(box.minima[0], xMin(part), 1e-6);
    EXPECT_NEAR(box.maxima[0], xMax(part), 1e-6);
  }
  if constexpr (D >= 2) {
    EXPECT_NEAR(box.minima[1], yMin(part), 1e-6);
    EXPECT_NEAR(box.maxima[1], yMax(part), 1e-6);
  }
  if constexpr (D >= 3) {
    EXPECT_NEAR(box.minima[2], zMin(part), 1e-6);
    EXPECT_NEAR(box.maxima[2], zMax(part), 1e-6);
  }
}

template <typename T, typename P>
UM2_HOSTDEV TEST_CASE(get_box_and_child)
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
  um2::RegularPartition<2, T, P> part;
  part.grid = grid;
  part.children.resize(32);
  for (len_t i = 0; i < 32; ++i) {
    part.children[i] = static_cast<P>(i);
  }
  um2::AABox2<T> box = part.getBox(0, 0);
  um2::AABox2<T> box_ref = {
      {         1,             -1},
      {one + half, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = part.getBox(1, 0);
  //{ { 1.5, -1.0 }, { 2.0, -0.75 } };
  box_ref = {
      {one + half,           -one},
      {       two, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = part.getBox(3, 0);
  // box_ref = { { 2.5, -1.0 }, { 3.0, -0.75 } };
  box_ref = {
      {two + half,           -one},
      {     three, -three * forth}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = part.getBox(0, 1);
  // box_ref = { { 1.0, -0.75 }, { 1.5, -0.5 } };
  box_ref = {
      {       one, -three * forth},
      {one + half,          -half}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = part.getBox(0, 7);
  // box_ref = { { 1.0, 0.75 }, { 1.5, 1.0 } };
  box_ref = {
      {       one, three * forth},
      {one + half,           one}
  };
  EXPECT_TRUE(isApprox(box, box_ref));
  box = part.getBox(3, 7);
  // box_ref = { { 2.5, 0.75 }, { 3.0, 1.0 } };
  box_ref = {
      {two + half, three * forth},
      {     three,           one}
  };
  EXPECT_TRUE(isApprox(box, box_ref));

  auto child = part.getChild(0, 0);
  EXPECT_EQ(child, 0);
  child = part.getChild(1, 0);
  EXPECT_EQ(child, 1);
  child = part.getChild(3, 0);
  EXPECT_EQ(child, 3);
  child = part.getChild(0, 1);
  EXPECT_EQ(child, 4);
  child = part.getChild(0, 7);
  EXPECT_EQ(child, 28);
  child = part.getChild(3, 7);
  EXPECT_EQ(child, 31);
}

template <std::floating_point T, std::integral I, len_t NFID>
TEST_CASE(quad_mesh_children)
{

  um2::QuadMesh<T, I> ref_mesh;
  makeQuadReferenceMesh(ref_mesh);
  EXPECT_TRUE(!ref_mesh.vertices.empty())
  using Child = um2::Vec<NFID, I>;
  um2::RegularPartition<2, T, Child> part = makePartEmpty<2, T, Child>();
  len_t const cell_nums = numCells(part.grid)[0] * numCells(part.grid)[1];
  part.children.resize(cell_nums);
  for (auto i = 0; i < cell_nums; ++i) {
    part.children[i].setConstant(-1);
  }
  EXPECT_TRUE(part.children.size() == cell_nums);
  part.set_child(ref_mesh);
  Eigen::Matrix<I, 4, NFID> const ref_children{
      {0, -1, -1, -1, -1, -1, -1, -1},
      {0,  1, -1, -1, -1, -1, -1, -1},
      {0, -1, -1, -1, -1, -1, -1, -1},
      {0,  1, -1, -1, -1, -1, -1, -1}
  };
  for (auto j = 0; j < cell_nums; j++) {
    for (auto i = 0; i < 8; i++) {
      EXPECT_EQ(part.children[j][i], ref_children(j, i));
    }
  }
}

template <std::floating_point T, std::integral I, len_t NFID>
TEST_CASE(tri_mesh_children)
{

  um2::TriMesh<T, I> ref_mesh;
  makeTriReferenceMesh(ref_mesh);
  EXPECT_TRUE(!ref_mesh.vertices.empty());
  using Child = um2::Vec<NFID, I>;
  um2::RegularPartition<2, T, Child> part = makePartEmpty<2, T, Child>();
  len_t cell_nums = numCells(part.grid)[0] * numCells(part.grid)[1];
  part.children.resize(cell_nums);
  for (auto i = 0; i < cell_nums; ++i) {
    part.children[i].setConstant(-1);
  }
  EXPECT_TRUE(part.children.size() == cell_nums);
  part.set_child(ref_mesh);
  for (auto j = 0; j < cell_nums; j++) {
    for (auto i = 0; i < 8; i++) {
      if (i != 0 && i != 1) {
        EXPECT_TRUE(part.children[j][i] == -1);
      } else {
        EXPECT_TRUE(part.children[j][i] == i);
      }
    }
  }
}

template <std::floating_point T, std::integral I, len_t NFID>
TEST_CASE(tri_mesh_children_boundary)
{

  um2::TriMesh<T, I> ref_mesh;
  makeTriReferenceMesh(ref_mesh);
  EXPECT_TRUE(!ref_mesh.vertices.empty());
  using Child = um2::Vec<NFID, I>;
  um2::RegularGrid<2, T> grid;
  grid.minima[0] = 0;
  grid.minima[1] = 0;
  grid.spacing[0] = 0.5;
  grid.spacing[1] = 0.5;
  grid.num_cells[0] = 2;
  grid.num_cells[1] = 2;
  um2::RegularPartition<2, T, Child> part;
  part.grid = grid;
  len_t cell_nums = numCells(part.grid)[0] * numCells(part.grid)[1];
  part.children.resize(cell_nums);
  for (auto i = 0; i < cell_nums; ++i) {
    part.children[i].setConstant(-1);
  }
  EXPECT_TRUE(part.children.size() == cell_nums);
  part.set_child(ref_mesh);
  // test the children as expected
}

#if UM2_ENABLE_CUDA
template <len_t D, typename T, typename P>
MAKE_CUDA_KERNEL(accessors, D, T, P)

template <len_t D, typename T, typename P>
MAKE_CUDA_KERNEL(bounding_box, D, T, P)

template <typename T, typename P>
MAKE_CUDA_KERNEL(get_box_and_child, T, P)

#endif

template <len_t D, typename T, typename P>
TEST_SUITE(regular_partition)
{
  TEST_HOSTDEV((accessors<D, T, P>))
  TEST_HOSTDEV((bounding_box<D, T, P>));
  if constexpr (D == 2) {
    TEST_HOSTDEV((get_box_and_child<T, P>));
  }
}

template <typename T, typename I>
TEST_SUITE(partition_children)
{
  TEST_HOSTDEV((tri_mesh_children<T, I, 8>))
  TEST_HOSTDEV((tri_mesh_children<T, I, 16>))
  TEST_HOSTDEV((quad_mesh_children<T, I, 8>))
  TEST_HOSTDEV((tri_mesh_children_boundary<T, I, 16>))
}

auto main() -> int
{
  RUN_TESTS((partition_children<float, int32_t>));
  RUN_TESTS((partition_children<double, int64_t>));
  RUN_TESTS((regular_partition<1, float, int32_t>));
  RUN_TESTS((regular_partition<2, float, int32_t>));
  RUN_TESTS((regular_partition<3, float, int32_t>));
  RUN_TESTS((regular_partition<1, double, uint64_t>));
  RUN_TESTS((regular_partition<2, double, uint64_t>));
  RUN_TESTS((regular_partition<3, double, uint64_t>));
  return 0;
}
