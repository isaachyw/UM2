namespace um2
{

template <len_t D, typename T, typename P>
template <len_t N>
constexpr void RegularPartition<D, T, P>::setChild(FaceVertexMesh<D, N, T, I> & mesh)
  requires(D == 2)
{
  for (auto i = 0; i < numFaces(mesh); i++) {
    //    get the bounding box of face i
    Vector<Point2<T>> face = getFace(mesh, i);
    AABox<2, T> box = boundingBox(face);
    Vector<T> range = this->grid.get_range_in_grid(box);
    if (range[0] >= 0) { // do have overlap with grid
      len_t i_min = static_cast<len_t>(range[0]);
      len_t i_max = static_cast<len_t>(range[1]);
      len_t j_min = static_cast<len_t>(range[2]);
      len_t j_max = static_cast<len_t>(range[3]);
      for (auto x_i = i_min; x_i <= i_max; x_i++) {
        for (auto j = j_min; j <= j_max; j++) {
          this->getChild(i, j).push_back(i);
        }
      }
    }
    //    set num of face id
  }
}

// -- Methods --

// Minima/maxima accessors.
template <len_t D, typename T, typename P>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMin(RegularPartition<D, T, P> const & part) -> T
{
  return xMin(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMin(RegularPartition<D, T, P> const & part) -> T
{
  return yMin(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMin(RegularPartition<D, T, P> const & part) -> T
{
  return zMin(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto xMax(RegularPartition<D, T, P> const & part) -> T
{
  return xMax(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto yMax(RegularPartition<D, T, P> const & part) -> T
{
  return yMax(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto zMax(RegularPartition<D, T, P> const & part) -> T
{
  return zMax(part.grid);
}

// Number of divisions accessors.
template <len_t D, typename T, typename P>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto numXcells(RegularPartition<D, T, P> const & part)
    -> len_t
{
  return numXcells(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto numYcells(RegularPartition<D, T, P> const & part)
    -> len_t
{
  return numYcells(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto numZcells(RegularPartition<D, T, P> const & part)
    -> len_t
{
  return numZcells(part.grid);
}

template <len_t D, typename T, typename P>
UM2_PURE UM2_HOSTDEV constexpr auto numCells(RegularPartition<D, T, P> const & part)
    -> Vec<D, len_t>
{
  return numCells(part.grid);
}

// Width/hight/depth
template <len_t D, typename T, typename P>
  requires(D >= 1)
UM2_PURE UM2_HOSTDEV constexpr auto width(RegularPartition<D, T, P> const & part) -> T
{
  return width(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 2)
UM2_PURE UM2_HOSTDEV constexpr auto height(RegularPartition<D, T, P> const & part) -> T
{
  return height(part.grid);
}

template <len_t D, typename T, typename P>
  requires(D >= 3)
UM2_PURE UM2_HOSTDEV constexpr auto depth(RegularPartition<D, T, P> const & part) -> T
{
  return depth(part.grid);
}

// Bounding box
template <len_t D, typename T, typename P>
UM2_PURE UM2_HOSTDEV constexpr auto boundingBox(RegularPartition<D, T, P> const & part)
    -> AABox<D, T>
{
  return boundingBox(part.grid);
}

template <len_t D, typename T, typename P>
UM2_NDEBUG_PURE UM2_HOSTDEV constexpr auto
RegularPartition<D, T, P>::getBox(len_t const i, len_t const j) const -> AABox2<T>
  requires(D == 2)
{
  return this->grid.getBox(i, j);
}

template <len_t D, typename T, typename P>
UM2_NDEBUG_PURE UM2_HOSTDEV constexpr auto
RegularPartition<D, T, P>::getChild(len_t const i, len_t const j) -> P &
  requires(D == 2)
{
  return this->children[j * numXcells(this->grid) + i];
}

template <len_t D, typename T, typename P>
UM2_NDEBUG_PURE UM2_HOSTDEV constexpr auto
RegularPartition<D, T, P>::getChild(len_t const i, len_t const j) const -> P const &
  requires(D == 2)
{
  return this->children[j * num_xcells(this->grid) + i];
}

} // namespace um2