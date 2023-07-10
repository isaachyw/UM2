#pragma once

#include <um2/common/color.hpp>
#include <um2/common/config.hpp>
#include <um2/common/vector.hpp>

namespace um2
{
struct Image {
  len_t nx, ny;
  Vector<Color> buffer;

  UM2_HOSTDEV explicit Image(len_t nx_in = 1080, len_t ny_in = 560) noexcept;

  UM2_HOSTDEV void to_ppm(String const & filename) const noexcept;
};

} // namespace um2

#include "image.inl"