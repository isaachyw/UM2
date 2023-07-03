namespace um2
{

// -- Constructors --

template <len_t D, typename T>
UM2_HOSTDEV constexpr RegularGrid<D, T>::RegularGrid(Point<D, T> const & minima_in,
                                                     Vec<D, T> const & spacing_in,
                                                     Vec<D, len_t> const & num_cells_in)
    : minima(minima_in),
      spacing(spacing_in),
      num_cells(num_cells_in)
{
  // Ensure all spacings and num_cells are positive
  for (len_t i = 0; i < D; ++i) {
    assert(spacing[i] > 0);
    assert(num_cells[i] > 0);
  }
}

template <len_t D, typename T>
UM2_HOSTDEV constexpr RegularGrid<D, T>::RegularGrid(AABox<D, T> const box)
    : minima(box.minima),
      spacing((box.maxima - box.minima).data)
{
  for (len_t i = 0; i < D; ++i) {
    num_cells[i] = 1;
  }
}

// Minima accessors.
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMin(RegularGrid<D, T> const & grid) -> T
{
  return grid.minima[0];
}

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMin(RegularGrid<D, T> const & grid) -> T
{
  return grid.minima[1];
}

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMin(RegularGrid<D, T> const & grid) -> T
{
  return grid.minima[2];
}

// Number of divisions accessors.
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto numXcells(RegularGrid<D, T> const & grid) -> len_t
{
  return grid.num_cells[0];
}

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto numYcells(RegularGrid<D, T> const & grid) -> len_t
{
  return grid.num_cells[1];
}

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto numZcells(RegularGrid<D, T> const & grid) -> len_t
{
  return grid.num_cells[2];
}

template <len_t D, typename T>
UM2_PURE UM2_HOSTDEV constexpr auto numCells(RegularGrid<D, T> const & grid)
    -> Vec<D, len_t>
{
  return {grid.num_cells};
}

// Width/hight/depth
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto width(RegularGrid<D, T> const & grid) -> T
{
  return grid.spacing[0] * static_cast<T>(numXcells(grid));
}

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto height(RegularGrid<D, T> const & grid) -> T
{
  return grid.spacing[1] * static_cast<T>(numYcells(grid));
}

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto depth(RegularGrid<D, T> const & grid) -> T
{
  return grid.spacing[2] * static_cast<T>(numZcells(grid));
}

// Maxima accessors.
template <len_t D, typename T>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMax(RegularGrid<D, T> const & grid) -> T
{
  return xMin(grid) + width(grid);
}

template <len_t D, typename T>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMax(RegularGrid<D, T> const & grid) -> T
{
  return yMin(grid) + height(grid);
}

template <len_t D, typename T>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMax(RegularGrid<D, T> const & grid) -> T
{
  return zMin(grid) + depth(grid);
}

// Bounding box
template <len_t D, typename T>
UM2_NDEBUG_PURE UM2_HOSTDEV constexpr auto boundingBox(RegularGrid<D, T> const & grid)
    -> AABox<D, T>
{
  assert(1 <= D && D <= 3);
  if constexpr (D == 1) {
    return AABox<D, T>(Point<D, T>(xMin(grid)), Point<D, T>(xMax(grid)));
  } else if constexpr (D == 2) {
    return AABox<D, T>(Point<D, T>(xMin(grid), yMin(grid)),
                       Point<D, T>(xMax(grid), yMax(grid)));
  } else {
    return AABox<D, T>(Point<D, T>(xMin(grid), yMin(grid), zMin(grid)),
                       Point<D, T>(xMax(grid), yMax(grid), zMax(grid)));
  }
}

template <len_t D, typename T>
UM2_NDEBUG_PURE UM2_HOSTDEV constexpr auto RegularGrid<D, T>::getBox(len_t const i,
                                                                     len_t const j) const
    -> AABox2<T>
  requires(D == 2)
{
  assert(i < numXcells(*this));
  assert(j < numYcells(*this));
  return AABox2<T>(Point2<T>(xMin(*this) + static_cast<T>(i) * this->spacing[0],
                             yMin(*this) + static_cast<T>(j) * this->spacing[1]),
                   Point2<T>(xMin(*this) + static_cast<T>(i + 1) * this->spacing[0],
                             yMin(*this) + static_cast<T>(j + 1) * this->spacing[1]));
}

template <len_t D, typename T>
auto RegularGrid<D, T>::get_range_in_grid(const AABox<2, T> & box) const -> Vec<4, len_t>
{
  T xmin = xMin(*this);
  T ymin = yMin(*this);
  auto i0 = static_cast<len_t>(std::floor((box.minima[0] - xmin) / this->spacing[0]));
  auto j0 = static_cast<len_t>(std::floor((box.minima[1] - ymin) / this->spacing[1]));
  auto i1 = static_cast<len_t>(std::ceil((box.maxima[0] - xmin) / this->spacing[0]));
  auto j1 = static_cast<len_t>(std::ceil((box.maxima[1] - ymin) / this->spacing[1]));
  // if completely outside the grid, return -1
  if (i1 < 0 || j1 < 0 || i0 >= numXcells(*this) || j0 >= numYcells(*this)) {
    return Vec<4, len_t>{-1, -1, -1, -1};
  }
  // consider the case when the box is partially outside the grid
  i0 = std::max<len_t>(i0, 0);
  j0 = std::max<len_t>(j0, 0);
  i1 = std::min<len_t>(i1, static_cast<len_t>(numXcells(*this)));
  j1 = std::min<len_t>(j1, static_cast<len_t>(numYcells(*this)));
  return Vec<4, len_t>{i0, j0, i1, j1};
}

} // namespace um2