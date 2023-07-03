#pragma once

#include <um2/common/config.hpp>             // UM2_HOSTDEV, UM2_PURE
#include <um2/geometry/axis_aligned_box.hpp> // AABox
#include <um2/geometry/point.hpp>            // Point

namespace um2
{

// -----------------------------------------------------------------------------
// REGULAR GRID
// -----------------------------------------------------------------------------
// A regular grid is a grid with a fixed spacing between points.

template <len_t D, typename T>
struct RegularGrid {

  // The bottom left corner of the grid.
  Point<D, T> minima;

  // The Δx, Δy, etc. of the grid.
  Vec<D, T> spacing;

  // The number of cells in each direction.
  // Must have at least 1 to form a grid.
  Vec<D, len_t> num_cells;

  // -- Constructors --

  UM2_HOSTDEV RegularGrid() = default;

  UM2_HOSTDEV constexpr RegularGrid(Point<D, T> const & /*minima_*/,
                                    Vec<D, T> const & /*spacing_*/,
                                    Vec<D, len_t> const & /*num_cells_*/);

  UM2_HOSTDEV constexpr explicit RegularGrid(AABox<D, T> /*box*/);

  // -- Methods --

  UM2_NDEBUG_PURE UM2_HOSTDEV [[nodiscard]] constexpr auto getBox(len_t i, len_t j) const
      -> AABox2<T>
    requires(D == 2);

  UM2_HOSTDEV [[nodiscard]] auto get_range_in_grid(AABox<2, T> const & /*box*/) const
      -> Vec<4, len_t>;
};

// -- Aliases --

template <typename T>
using RegularGrid1 = RegularGrid<1, T>;
template <typename T>
using RegularGrid2 = RegularGrid<2, T>;
template <typename T>
using RegularGrid3 = RegularGrid<3, T>;

using RegularGrid1f = RegularGrid1<float>;
using RegularGrid2f = RegularGrid2<float>;
using RegularGrid3f = RegularGrid3<float>;

using RegularGrid1d = RegularGrid1<double>;
using RegularGrid2d = RegularGrid2<double>;
using RegularGrid3d = RegularGrid3<double>;

// -- Methods --

// Minima/maxima accessors.
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMin(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMin(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMin(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMax(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMax(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMax(RegularGrid<D, T> const & /*grid*/) -> T;

// Number of cells
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto numXcells(RegularGrid<D, T> const & /*grid*/)
    -> len_t;

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto numYcells(RegularGrid<D, T> const & /*grid*/)
    -> len_t;

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto numZcells(RegularGrid<D, T> const & /*grid*/)
    -> len_t;

template <len_t D, typename T>
UM2_PURE UM2_HOSTDEV constexpr auto numCells(RegularGrid<D, T> const & /*grid*/)
    -> Vec<D, len_t>;

// Width/hight/depth
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto width(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto height(RegularGrid<D, T> const & /*grid*/) -> T;

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto depth(RegularGrid<D, T> const & /*grid*/) -> T;

// Bounding box
template <len_t D, typename T>
UM2_PURE UM2_HOSTDEV constexpr auto boundingBox(RegularGrid<D, T> const & /*grid*/)
    -> AABox<D, T>;

} // namespace um2

#include "regular_grid.inl"