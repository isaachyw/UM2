template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeTriReferenceMesh(um2::TriMesh<T, I> & mesh)
{
  mesh.vertices = {
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1}
  };
  mesh.fv = {0, 1, 2, 2, 3, 0};
  mesh.vf_offsets = {0, 2, 3, 5, 6};
  mesh.vf = {0, 1, 0, 0, 1, 1};
}

template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeQuadReferenceMesh(um2::QuadMesh<T, I> & mesh)
{
  mesh.vertices = {
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1},
      {2, 0},
      {2, 1}
  };
  mesh.fv = {0, 1, 2, 3, 1, 4, 5, 2};
  mesh.vf_offsets = {0, 1, 3, 5, 6, 7, 8};
  mesh.vf = {0, 0, 1, 0, 1, 0, 1, 1};
}

template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeTriQuadReferenceMesh(um2::TriQuadMesh<T, I> & mesh)
{
  mesh.vertices = {
      {0, 0},
      {1, 0},
      {1, 1},
      {0, 1},
      {2, 0}
  };
  mesh.fv_offsets = {0, 4, 7};
  mesh.fv = {0, 1, 2, 3, 1, 4, 2};
  mesh.vf_offsets = {0, 1, 3, 5, 6, 7};
  mesh.vf = {0, 0, 1, 0, 1, 0, 1};
}

template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeTri6ReferenceMesh(um2::QuadraticTriMesh<T, I> & mesh)
{
  mesh.vertices = {
      {                  0,                   0},
      {                  1,                   0},
      {                  0,                   1},
      {static_cast<T>(0.5), static_cast<T>(0.0)},
      {static_cast<T>(0.7), static_cast<T>(0.5)},
      {static_cast<T>(0.0), static_cast<T>(0.5)},
      {                  1,                   1},
      {static_cast<T>(1.0), static_cast<T>(0.5)},
      {static_cast<T>(0.5), static_cast<T>(1.0)}
  };
  mesh.fv = {0, 1, 2, 3, 4, 5, 1, 6, 2, 7, 8, 4};
  mesh.vf_offsets = {0, 1, 3, 5, 6, 8, 9, 10, 11, 12};
  mesh.vf = {0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1};
}

template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeQuad8ReferenceMesh(um2::QuadraticQuadMesh<T, I> & mesh)
{
  mesh.vertices = {
      {static_cast<T>(0.0), static_cast<T>(0.0)},
      {static_cast<T>(1.0), static_cast<T>(0.0)},
      {static_cast<T>(1.0), static_cast<T>(1.0)},
      {static_cast<T>(0.0), static_cast<T>(1.0)},
      {static_cast<T>(2.0), static_cast<T>(0.0)},
      {static_cast<T>(2.0), static_cast<T>(1.0)},
      {static_cast<T>(0.5), static_cast<T>(0.0)},
      {static_cast<T>(1.1), static_cast<T>(0.6)},
      {static_cast<T>(0.5), static_cast<T>(1.0)},
      {static_cast<T>(0.0), static_cast<T>(0.5)},
      {static_cast<T>(1.5), static_cast<T>(0.0)},
      {static_cast<T>(2.0), static_cast<T>(0.5)},
      {static_cast<T>(1.5), static_cast<T>(1.0)}
  };
  mesh.fv = {0, 1, 2, 3, 6, 7, 8, 9, 1, 4, 5, 2, 10, 11, 12, 7};
  mesh.vf_offsets = {0, 1, 3, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16};
  mesh.vf = {0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1};
}

template <std::floating_point T, std::signed_integral I>
UM2_HOSTDEV static void makeTri6Quad8ReferenceMesh(um2::QuadraticTriQuadMesh<T, I> & mesh)
{
  mesh.vertices = {
      {static_cast<T>(0.0), static_cast<T>(0.0)},
      {static_cast<T>(1.0), static_cast<T>(0.0)},
      {static_cast<T>(1.0), static_cast<T>(1.0)},
      {static_cast<T>(0.0), static_cast<T>(1.0)},
      {static_cast<T>(2.0), static_cast<T>(0.0)},
      {static_cast<T>(0.5), static_cast<T>(0.0)},
      {static_cast<T>(0.7), static_cast<T>(0.6)},
      {static_cast<T>(0.5), static_cast<T>(1.0)},
      {static_cast<T>(0.0), static_cast<T>(0.5)},
      {static_cast<T>(1.5), static_cast<T>(0.0)},
      {static_cast<T>(1.5), static_cast<T>(0.5)}
  };
  mesh.fv_offsets = {0, 8, 14};
  mesh.fv = {0, 1, 2, 3, 5, 6, 7, 8, 1, 4, 2, 9, 10, 6};
  mesh.vf_offsets = {0, 1, 3, 5, 6, 7, 8, 10, 11, 12, 13, 14};
  mesh.vf = {0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1};
}