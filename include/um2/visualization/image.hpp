#pragma once

#include <um2/common/color.hpp>
#include <um2/common/config.hpp>
#include <um2/common/vector.hpp>
#include <um2/mesh/face_vertex_mesh.hpp>
namespace um2
{
struct Image {
  static const Vector<Color> prop_cycle;
  static constexpr len_t pixel_density = 270;
  len_t nx, ny;
  Vector<Color> buffer;

  UM2_HOSTDEV explicit Image(len_t nx_in = 1080, len_t ny_in = 560) noexcept;

  UM2_HOSTDEV [[nodiscard]] constexpr auto getPixel(len_t i, len_t j) const noexcept
      -> Color;

  UM2_HOSTDEV void to_ppm(String const & filename) const noexcept;

  template <len_t P, len_t N, std::floating_point T, std::signed_integral I>
  UM2_HOSTDEV explicit Image(um2::FaceVertexMesh<P, N, T, I> const & mesh) noexcept;
};

} // namespace um2

#include "image.inl"