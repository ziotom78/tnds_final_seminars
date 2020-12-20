#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdio>
#include <vector>

#if __cplusplus < 202001
// No C++20 support
#include "fmt/format.h"
#else
// We're in the future!
#include <format>
namespace fmt = std::format;
#endif

using namespace std;

template <typename T> double abs2(complex<T> z) {
  return z.real() * z.real() + z.imag() * z.imag();
}

/* Return -1 if the point "start" belongs to the Julia set
 * parametrized by "c". Otherwise, return the number of iterations
 * needed to find that the limit diverges. Do not iterate more than
 * "max_iter" times, so that the return value of this function is
 * always in the range [-1, max_iter].
 */
int julia(complex<double> start, complex<double> c, int max_iter) {
  int iter = 0;
  complex<double> z = start;

  while (abs2(z) < 4.0 && iter < max_iter) {
    z = z * z + c;

    iter++;
  }

  return (iter >= max_iter) ? -1 : iter;
}

/* Apply a linear transformation to "val", so that the following
 * applies:
 *
 * - If "val" == 0, then "out_min" is returned;
 *
 * - If "val" == max - 1, then "out_max" is returned;
 *
 * - For any other value, return a linear interpolation between the
 *   two extrema
 */
double linspace(int val, int max, double out_min, double out_max) {
  return out_min + (val * (out_max - out_min)) / (max - 1);
}

/* Apply the function "julia" to a matrix of points in
 * the subdomain [x0, x1] x [y0, y1] of the complex plane.
 * The size of the matrix is num_of_cols * num_of_rows, and
 * it is saved in the array "image_matrix" (row-first order).
 */
void juliaset(complex<double> c, double x0, double x1, int num_of_cols,
              double y0, double y1, int num_of_rows, int max_iter,
              int *image_matrix) {
  int idx = 0;
  for (int row = 0; row < num_of_rows; ++row) {
    for (int col = 0; col < num_of_cols; ++col) {
      complex<double> z(linspace(col, num_of_cols, x0, x1),
                        linspace(row, num_of_rows, y0, y1));

      image_matrix[idx++] = julia(z, c, max_iter);
    }
  }
}

int main(void) {
  const int width = 800;
  const int height = 800;
  const int maxiter = 10000;
  const char *file_name = "julia-fractal.pgm";
  const complex<double> c(0.2, 0.55);

  std::vector<int> image(width * height);

  auto start = std::chrono::steady_clock::now();
  // Compute the fractal
  juliaset(c, -1.5, 1.5, width, -1.5, 1.5, height, maxiter, image.data());
  auto end = std::chrono::steady_clock::now();

  fmt::print(stderr, "Time required to compute the fractal: {0} ms\n",
             std::chrono::duration<double, std::milli>(end - start).count());

  // Normalize the image
  int max_value = *std::max_element(image.begin(), image.end());

  /* Save the image in the NetPbm "Portable GrayMap" format
   * (https://en.wikipedia.org/wiki/Netpbm_format) */
  FILE *output_file = fopen(file_name, "wt");
  assert(output_file != nullptr);
  fmt::print(output_file,
             "P2\n"
             "# Julia set for the point {0} + {1}j\n"
             "{2} {3}\n"
             "255\n",
             c.real(), c.imag(), width, height);

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      double val;
      if (image[j * height + i] < 0)
        val = 0.0;
      else {
        // Using the "pow" functions corrects the gamma of the image,
        // making the Julia set brighter
        val = std::pow(1.0 - (image[j * height + i] * 1.0) / max_value, 5.0);
      }

      fmt::print(output_file, "{} ", (int)((val * 255.0) + 0.5));
    }
    fmt::print(output_file, "\n");
  }

  fclose(output_file);
  fmt::print(stderr, "Image written to file \"{}\"\n", file_name);

  return 0;
}
