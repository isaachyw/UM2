namespace um2
{

// -- Constructors --

template <len_t D, typename T>
UM2_HOSTDEV constexpr Ray<D, T>::Ray(Point<D, T> const & origin,
                                     Point<D, T> const & direction)
    : o(origin),
      d(direction)
{
  assert(abs((direction.norm() - static_cast<T>(1))) < static_cast<T>(1e-5));
}

} // namespace um2