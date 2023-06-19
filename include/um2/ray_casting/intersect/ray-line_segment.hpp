#pragma once

#include <um2/geometry/line_segment.hpp>
#include <um2/geometry/polytope.hpp>
#include <um2/ray_casting/ray.hpp>

#include <concepts>

namespace um2
{

template <std::floating_point T>
UM2_PURE constexpr auto intersect(Ray2<T> const & ray, LineSegment<2, T> const & line)
    -> T
{
  auto const v = line[1] - line[0];
  auto const u = ray.o - line[0];

  auto const x = cross2(u, ray.d);
  auto const z = cross2(v, ray.d);
  auto const y = cross2(u, v);

  auto const s = x / z;
  auto const r = y / z;

  return 0 <= s && s <= 1 ? r : infiniteDistance<T>();
}

template <std::floating_point T>
UM2_PURE constexpr auto rayLineSegmentIntersection(Ray2<T> const & ray,
                                                   Point2<T> const & l0,
                                                   Point2<T> const & l1) -> T
{
  LineSegment<2, T> line(l0, l1);
  return intersect(ray, line);
}

} // namespace um2