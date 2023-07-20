#include <fstream>
#include <iostream>
#include <um2/geometry/polytope.hpp>
#include <um2/geometry/quadrilateral.hpp>
#include <um2/geometry/triangle.hpp>
#include <um2/mesh/regular_partition.hpp>

namespace um2
{

const Vector<Color> Image::prop_cycle{
    Color(31, 119, 180),  Color(255, 127, 14), Color(44, 160, 44),   Color(214, 39, 40),
    Color(148, 103, 189), Color(140, 86, 75),  Color(227, 119, 194), Color(127, 127, 127),
    Color(188, 189, 34),  Color(23, 190, 207)};

// -----------------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------------

UM2_HOSTDEV Image::Image(len_t nx_in, len_t ny_in) noexcept
    : nx(nx_in),
      ny(ny_in),
      buffer(nx_in * ny_in, Color(124, 124, 124))
{
}

void Image::to_ppm(const String & filename) const noexcept
{
  std::ofstream ofs(filename.begin(), std::ios::out | std::ios::binary);
  ofs << "P6\n" << this->nx << " " << this->ny << "\n255\n";
  for (len_t i = 0; i < ny; i++) {
    for (len_t j = 0; j < nx; j++) {
      um2::Color const & pixel = buffer[i * nx + j];
      ofs << bit_cast<char>(pixel.r) << bit_cast<char>(pixel.g)
          << bit_cast<char>(pixel.b);
    }
  }
  ofs.close();
}

constexpr auto Image::getPixel(len_t i, len_t j) const noexcept -> Color
{
  return this->buffer[i * nx + j];
}

// NOLINTBEGIN
template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
Image::Image(const FaceVertexMesh<P, N, T, I> & mesh, um2::Color edge_color,
             um2::Color vertex_color, T vertex_radius) noexcept
{
  len_t grid_size = 2;
  AABox2<T> const & bbox = boundingBox(mesh);
  T width = bbox.width();
  T height = bbox.height();
  this->nx = static_cast<len_t>(pixel_density * width);
  this->ny = static_cast<len_t>(pixel_density * height);
  this->buffer.resize(nx * ny);
  // set the regular partition children
  RegularPartition<2, T, Vec<8, I>> partition;
  //  try 4 by 4 grid, todo: benchmark different granularity of the grid
  partition.grid = RegularGrid<2, T>(bbox.minima, {width / grid_size, height / grid_size},
                                     {grid_size, grid_size});
  partition.children.resize(grid_size * grid_size);
  for (len_t i = 0; i < grid_size * grid_size; i++) {
    partition.children[i].setConstant(-1);
  }
  partition.set_child(mesh);
#ifdef _OPENMP
#  pragma omp parallel for
#endif
  for (len_t i_grid = 0; i_grid < grid_size; i_grid++) {
    for (len_t j_grid = 0; j_grid < grid_size; j_grid++) {
      len_t i_min = i_grid * ny / grid_size;
      len_t i_max = (i_grid + 1) * ny / grid_size;
      len_t j_min = j_grid * nx / grid_size;
      len_t j_max = (j_grid + 1) * nx / grid_size;
      Vec<8, I> face_indices = partition.children[i_grid * grid_size + j_grid];
      len_t last_face = -1;
      // loop all the pixels in the grid cell
      for (len_t i = i_min; i < i_max; i++) {
        for (len_t j = j_min; j < j_max; j++) {
          //           first test if it is in the last face
          if (last_face != -1) {
            auto face = getFace(mesh, face_indices[last_face]);
            if (face.size() == 3) {
              Triangle<2, T> tri(face[0], face[1], face[2]);
              if (tri.contains(Vec<2, T>(bbox.minima[0] + j * width / nx,
                                         bbox.minima[1] + i * height / ny))) {
                this->buffer[i * nx + j] =
                    prop_cycle[face_indices[last_face] % prop_cycle.size()];
                continue;
              }
            } else if (face.size() == 4) {
              Quadrilateral<2, T> quad(face[0], face[1], face[2], face[3]);
              if (quad.contains(Vec<2, T>(bbox.minima[0] + j * width / nx,
                                          bbox.minima[1] + i * height / ny))) {
                this->buffer[i * nx + j] =
                    prop_cycle[face_indices[last_face] % prop_cycle.size()];
                continue;
              }
            }
          }
          for (len_t f_id = 0; f_id < face_indices.size(); f_id++) {
            if (face_indices[f_id] == -1) {
              continue;
            }
            auto face = getFace(mesh, face_indices[f_id]);
            if (face.size() == 3) {
              Triangle<2, T> tri(face[0], face[1], face[2]);
              if (tri.contains(Vec<2, T>(bbox.minima[0] + j * width / nx,
                                         bbox.minima[1] + i * height / ny))) {
                this->buffer[i * nx + j] =
                    prop_cycle[face_indices[f_id] % prop_cycle.size()];
                last_face = f_id;
                break; // break out of the face loop
              }
            } else if (face.size() == 4) {
              Quadrilateral<2, T> quad(face[0], face[1], face[2], face[3]);
              if (quad.contains(Vec<2, T>(bbox.minima[0] + j * width / nx,
                                          bbox.minima[1] + i * height / ny))) {
                this->buffer[i * nx + j] =
                    prop_cycle[face_indices[f_id] % prop_cycle.size()];
                last_face = f_id;
                break; // break out of the face loop
              }
            }
          }
        }
      }
    }
  }

  /***************************************************************************
   *
   * render edge, use Bresenham's line algorithm
   */
  Vector<std::pair<I, I>> edges = getEdge(mesh);
#ifdef _OPENMP
#  pragma omp parallel for
#endif
  for (len_t i = 0; i < edges.size(); i++) {
    Vec<2, T> p0 = mesh.vertices[edges[i].first];
    Vec<2, T> p1 = mesh.vertices[edges[i].second];
    len_t x0 =
        len_t(std::floor(static_cast<double>((p0[0] - bbox.minima[0]) * nx) / width));
    len_t y0 =
        len_t(std::floor(static_cast<double>((p0[1] - bbox.minima[1]) * ny) / height));
    len_t x1 =
        len_t(std::floor(static_cast<double>((p1[0] - bbox.minima[0]) * nx) / width));
    len_t y1 =
        len_t(std::floor(static_cast<double>((p1[1] - bbox.minima[1]) * ny) / height));
    x0 = x0 == nx ? nx - 1 : x0;
    y0 = y0 == ny ? ny - 1 : y0;
    x1 = x1 == nx ? nx - 1 : x1;
    y1 = y1 == ny ? ny - 1 : y1;
    len_t dx = std::abs(x1 - x0);
    len_t dy = std::abs(y1 - y0);
    len_t sx = (x0 < x1) ? 1 : -1;
    len_t sy = (y0 < y1) ? 1 : -1;
    len_t err = dx - dy;
    while (true) {
      this->buffer[y0 * nx + x0] = edge_color;
      if ((x0 == x1) && (y0 == y1)) {
        break;
      }
      len_t e2 = 2 * err;
      if (e2 > -dy) {
        err -= dy;
        x0 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
  // finally render the vertices, use a square, radius = 20
  len_t radius = static_cast<len_t>(pixel_density * vertex_radius);
#ifdef _OPENMP
#  pragma omp parallel for
#endif
  for (auto idx = 0; idx < mesh.vertices.size(); idx++) {
    len_t x0 = len_t(std::floor(
        static_cast<double>((mesh.vertices[idx][0] - bbox.minima[0]) * nx) / width));
    len_t y0 = len_t(std::floor(
        static_cast<double>((mesh.vertices[idx][1] - bbox.minima[1]) * ny) / height));
    for (len_t i = x0 - radius; i < x0 + radius; i++) {
      for (len_t j = y0 - radius; j < y0 + radius; j++) {
        if (i >= 0 && i < nx && j >= 0 && j < ny) {
          this->buffer[j * nx + i] = vertex_color;
        }
      }
    }
    // render all the pixel in the square surrounding the vertex in the buffer
  }
  // NOLINTEND
}
} // namespace um2