#pragma once

#include <um2/common/config.hpp>
#include <um2/geometry/axis_aligned_box.hpp>
#include <um2/mesh/cell_type.hpp>
// #include <um2/mesh/mesh_file.hpp>

#include <concepts>
#include <iostream>
#include <type_traits>

namespace um2
{

// ASSUMES ALL POINTS HAVE POSITIVE COORDINATES

// FACE-VERTEX MESH
//-----------------------------------------------------------------------------
// A 2D volumetric mesh composed of polygons of polynomial order P. Each polygon
//(face) is composed of N vertices. Each vertex is a 2D point of floating point
// type T. For heterogeneous meshes, N is the sum of the number of vertices of
// each face. This makes each mesh type uniquely identifiable by P and N as follows:
//  - P = 1, N =  3: Triangular mesh
//  - P = 1, N =  4: Quadrilateral mesh
//  - P = 1, N =  7: Tri + quad mesh
//  - P = 2, N =  6: Quadratic triangular mesh
//  - P = 2, N =  8: Quadratic quadrilateral mesh
//  - P = 2, N = 14: Quadratic Tri + quad mesh
// Let I be the signed integer type used to index vertices and faces.
// We will use some simple meshes to explain the data structure. A more detailed
// explanation of each member follows.
//  - A TriMesh (FaceVertexMesh<1, 3>) with two triangles:
//      3---2
//      | / |
//      0---1
//      vertices = { {0, 0}, {1, 0}, {1, 1}, {0, 1} }
//          4 vertices on the unit square
//      fv = {0, 1, 2, 2, 3, 0}
//          The 6 vertex indices composing the two triangles {0, 1, 2} and {2, 3, 0}
//      fv_offsets = {}
//          fv_offsets[i] is the index of the first vertex of face i in fv.
//          For N = 3, this is always empty, since each face has 3 vertices,
//          hence the offsets are always 0, 3, 6, etc.
//      vf = { 0, 1, 0, 0, 1, 1 }
//          The face indices to which each vertex belongs. More precisely, vertex
//          0 belongs to faces 0 and 1, vertex 1 belongs to face 0 only, etc.
//          Face IDs are ordered least to greatest.
//      vf_offsets = { 0, 2, 3, 5, 6}
//          vf_offsets[i] is the index of the smallest face ID to which vertex i
//          belongs. There is an additional element at the end, which is the length
//          of the vf vector. Used to calculate the number of faces to which each
//          vertex belongs.
//  - A TriQuadMesh (FaceVertexMesh<1, 7>) with one triangle and one quad:
//    3---2
//    |   | \                                                                       .
//    0---1---4
//      vertices = { {0, 0}, {1, 0}, {1, 1}, {0, 1}, {2, 0} }
//          5 vertices on the unit square and one on the unit line
//      fv = {0, 1, 2, 3, 1, 4, 2}
//          The 7 vertex indices composing the quad {0, 1, 2, 3} and the triangle
//          {1, 4, 2}
//      fv_offsets = {0, 4, 7}
//          Face 0 starts at fv[0] and has 4 - 0 = 4 vertices. Face 1 starts at
//          fv[4] and has 7 - 4 = 3 vertices.
//      vf = { 0, 0, 1, 0, 1, 0, 1 }
//          v0 belongs to face 0, v1 belongs to faces 0 and 1, v2 belongs to faces
//          0 and 1, v3 belongs to face 0, v4 belongs to face 1.
//      vf_offsets = { 0, 1, 3, 5, 6, 7 }
//          v0's connectivity starts at vf[0] and has 1 - 0 = 1 face. v1's
//          connectivity starts at vf[1] and has 3 - 1 = 2 faces. etc.
// For homogeneous meshes, fv_offsets is empty.
// For heterogeneous meshes, fv_offsets is non-empty.

template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
struct FaceVertexMesh {

  Vector<Point2<T>> vertices;
  Vector<I> fv_offsets; // size = num_faces + 1
  Vector<I> fv;         // size = fv_offsets[num_faces]
  Vector<I> vf_offsets; // size = num_vertices + 1
  Vector<I> vf;         // size = vf_offsets[num_vertices]

  // -- Constructors --

  UM2_HOSTDEV FaceVertexMesh() = default;
  //  FaceVertexMesh(MeshFile<T, I> const &);

  // -- Methods --

  //  void to_mesh_file(MeshFile<T, I> &) const;
};

// Aliases
// -----------------------------------------------------------------------------

template <len_t N, std::floating_point T, std::signed_integral I>
using LinearPolygonMesh = FaceVertexMesh<1, N, T, I>;

template <len_t N, std::floating_point T, std::signed_integral I>
using QuadraticPolygonMesh = FaceVertexMesh<2, N, T, I>;

template <std::floating_point T, std::signed_integral I>
using TriMesh = LinearPolygonMesh<3, T, I>;

template <std::floating_point T, std::signed_integral I>
using QuadMesh = LinearPolygonMesh<4, T, I>;

template <std::floating_point T, std::signed_integral I>
using TriQuadMesh = LinearPolygonMesh<7, T, I>;

template <std::floating_point T, std::signed_integral I>
using QuadraticTriMesh = QuadraticPolygonMesh<6, T, I>;

template <std::floating_point T, std::signed_integral I>
using QuadraticQuadMesh = QuadraticPolygonMesh<8, T, I>;

template <std::floating_point T, std::signed_integral I>
using QuadraticTriQuadMesh = QuadraticPolygonMesh<14, T, I>;

template <typename I>
struct ConditionalCast {
  using Type = std::conditional_t<std::is_same_v<I, len_t>, I, len_t>;
};

// -- Methods --

template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
UM2_NDEBUG_PURE auto numFaces(FaceVertexMesh<P, N, T, I> const & mesh) -> len_t
{
  if constexpr ((P == 1 && N == 7) || (P == 2 && N == 14)) {
    return mesh.fv_offsets.size() - 1;
  } else {
    return mesh.fv.size() / N;
  }
}

// NOLINTBEGIN
template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
UM2_NDEBUG_PURE auto getEdge(FaceVertexMesh<P, N, T, I> const & mesh)
    -> Vector<std::pair<I, I>>
{
  Vector<std::pair<I, I>> edges;
  Vector<std::pair<I, I>> unique_edges;
  if (!mesh.fv_offsets.empty()) {
    for (len_t i = 0;
         i < static_cast<typename ConditionalCast<I>::Type>(mesh.fv_offsets.size() - 1);
         i++) {
      I next_offset =
          mesh.fv_offsets[static_cast<typename ConditionalCast<I>::Type>(i + 1)] - 1;
      for (len_t j = static_cast<typename ConditionalCast<I>::Type>(mesh.fv_offsets[i]);
           j < static_cast<typename ConditionalCast<I>::Type>(next_offset); ++j) {
        if (mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j)] <
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j) + 1]) {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j)],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j) + 1]));
        } else {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j) + 1],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(j)]));
        }
      }
      if (mesh.fv[static_cast<typename ConditionalCast<I>::Type>(next_offset)] <
          mesh.fv[static_cast<typename ConditionalCast<I>::Type>(mesh.fv_offsets[i])]) {
        edges.push_back(std::make_pair(
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(next_offset)],
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(mesh.fv_offsets[i])]));
      } else {
        edges.push_back(std::make_pair(
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(mesh.fv_offsets[i])],
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(next_offset)]));
      }
    }
  } else {
    for (len_t i = 0; i < static_cast<typename ConditionalCast<I>::Type>(mesh.fv.size());
         ++i) {
      if (i % N != static_cast<typename ConditionalCast<I>::Type>(N - 1)) {
        if (mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)] <
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) + 1]) {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) + 1]));
        } else {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) + 1],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)]));
        }
      } else {
        if (mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)] <
            mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) -
                    static_cast<typename ConditionalCast<I>::Type>(N - 1)]) {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) -
                      static_cast<typename ConditionalCast<I>::Type>(N - 1)]));
        } else {
          edges.push_back(std::make_pair(
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i) -
                      static_cast<typename ConditionalCast<I>::Type>(N - 1)],
              mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)]));
        }
      }
    }
  }
  unique_edges.reserve(edges.size());
  for (auto e : edges) {
    if (std::find(unique_edges.begin(), unique_edges.end(), e) == unique_edges.end()) {
      unique_edges.push_back(e);
    }
  }

  return unique_edges;
}

template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
UM2_NDEBUG_PURE auto getFace(FaceVertexMesh<P, N, T, I> const & mesh, len_t id)
    -> Vector<Point2<T>>
{
  Vector<Point2<T>> vertices;
  if (!mesh.fv_offsets.empty()) {
    for (auto i = mesh.fv_offsets[id]; i < mesh.fv_offsets[id + 1]; ++i) {
      vertices.push_back(mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
          mesh.fv[static_cast<typename ConditionalCast<I>::Type>(i)])]);
    }
  } else {
    for (auto i = id * N; i < (id + 1) * N; ++i) {
      vertices.push_back(
          mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(mesh.fv[i])]);
    }
  }
  return vertices;
}

// NOLINTEND

template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
UM2_NDEBUG_PURE auto boundingBox(FaceVertexMesh<P, N, T, I> const &) -> AABox2<T>;

//////// -- STATISTICS --

template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
void printMeshStats(FaceVertexMesh<P, N, T, I> const & mesh)
{
  Vector<std::pair<I, I>> edges = getEdge(mesh);
  // find the maximum and minimum edge length
  auto max_edge = std::max_element(
      edges.begin(), edges.end(),
      [&mesh](std::pair<I, I> const & a, std::pair<I, I> const & b) {
        return um2::distance(
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(a.first)],
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                       a.second)]) <
               um2::distance(
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(b.first)],
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                       b.second)]);
      });
  auto min_edge = std::max_element(
      edges.begin(), edges.end(),
      [&mesh](std::pair<I, I> const & a, std::pair<I, I> const & b) {
        return um2::distance(
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(a.first)],
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                       a.second)]) >=
               um2::distance(
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(b.first)],
                   mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                       b.second)]);
      });
  T average_edge_length = 0;
  for (auto & e : edges) {
    average_edge_length += um2::distance(
        mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(e.first)],
        mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(e.second)]);
  }
  std::cout << "Mesh statistics:\n";
  std::cout << "  Number of vertices: " << mesh.vertices.size() << "\n";
  std::cout << "  Average edge length: "
            << average_edge_length / static_cast<T>(edges.size()) << "\n";
  std::cout << "  Minimum edge length: "
            << um2::distance(mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                                 min_edge->first)],
                             mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                                 min_edge->second)])
            << "\n";
  std::cout << "  Maximum edge length: "
            << um2::distance(mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                                 max_edge->first)],
                             mesh.vertices[static_cast<typename ConditionalCast<I>::Type>(
                                 max_edge->second)])
            << "\n";
  std::cout << "End of mesh statistics\n";
}

////// -- IO --
////
////template <len_t K, len_t P, len_t N, len_t D, typename T>
////std::ostream & operator << (std::ostream &, Polytope<K, P, N, D, T> const &);
//
} // namespace um2

#include "face_vertex_mesh.inl"