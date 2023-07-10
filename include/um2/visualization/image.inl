#include <fstream>
namespace um2
{

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
} // namespace um2