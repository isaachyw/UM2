#pragma once

#include <um2/geometry/Polygon.hpp>
#include <um2/geometry/Quadrilateral.hpp>

namespace um2
{

//==============================================================================
// QUADRATIC QUADRILATERAL
//==============================================================================
//
// A 2-polytope, of polynomial order 2, represented by the connectivity
// of its vertices. These 8 vertices are D-dimensional points of type T.

template <Size D, typename T>
struct Polytope<2, 2, 8, D, T> {

  Point<D, T> v[8];

  //==============================================================================
  // Constructors
  //==============================================================================

  constexpr Polytope() noexcept = default;

  HOSTDEV constexpr Polytope(Point<D, T> const & p0, Point<D, T> const & p1,
                             Point<D, T> const & p2, Point<D, T> const & p3,
                             Point<D, T> const & p4, Point<D, T> const & p5,
                             Point<D, T> const & p6, Point<D, T> const & p7) noexcept;

  //==============================================================================
  // Accessors
  //==============================================================================

  PURE HOSTDEV constexpr auto
  operator[](Size i) noexcept -> Point<D, T> &;

  PURE HOSTDEV constexpr auto
  operator[](Size i) const noexcept -> Point<D, T> const &;

  //==============================================================================
  // Methods
  //==============================================================================

  template <typename R, typename S>
  PURE HOSTDEV [[nodiscard]] constexpr auto
  operator()(R r, S s) const noexcept -> Point<D, T>;

  template <typename R, typename S>
  PURE HOSTDEV [[nodiscard]] constexpr auto
  jacobian(R r, S s) const noexcept -> Mat<D, 2, T>;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  getEdge(Size i) const noexcept -> QuadraticSegment<D, T>;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  contains(Point<D, T> const & p) const noexcept -> bool;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  linearPolygon() const noexcept -> Quadrilateral<D, T>;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  area() const noexcept -> T;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  centroid() const noexcept -> Point<D, T>;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  boundingBox() const noexcept -> AxisAlignedBox<D, T>;

  PURE HOSTDEV [[nodiscard]] constexpr auto
  isCCW() const noexcept -> bool;
};

} // namespace um2

#include "QuadraticQuadrilateral.inl"
