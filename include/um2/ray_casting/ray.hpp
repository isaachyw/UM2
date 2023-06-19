#pragma once

#include <spdlog/spdlog.h>        // UM2_ASSERT
#include <um2/common/config.hpp>  // UM2_CONST, UM2_PURE, UM2_HOSTDEV
#include <um2/geometry/point.hpp> // Point

namespace um2
{

template <len_t D, typename T>
struct Ray {

  Point<D, T> o; // origin
  Point<D, T> d; // direction (unit vector)

  // -- Constructors --

  UM2_HOSTDEV constexpr Ray(Point<D, T> const & /*origin*/,
                            Point<D, T> const & /*direction*/);

}; // struct Ray

// -- Aliases --

template <typename T>
using Ray1 = Ray<1, T>;

template <typename T>
using Ray2 = Ray<2, T>;

using Ray2f = Ray2<float>;
using Ray2d = Ray2<double>;

} // namespace um2

#include "ray.inl"