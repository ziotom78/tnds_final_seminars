#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
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

/* Return -1 if the point (start_x) + (start_y)*i belongs to the
 * Julia set parametrized by (c_x) + (c_y)*i. Otherwise, return
 * the number of iterations needed to find that the limit diverges.
 * Do not iterate more than "max_iter" times, so that the return
 * value of this function is always in the range [-1, max_iter].
 */
int julia(double start_x, double start_y, double c_x, double c_y,
          int max_iter) {
  int iter = 0;
  double z_x = start_x;
  double z_y = start_y;

  while ((z_x * z_x + z_y * z_y < 4.0) && (iter < max_iter)) {
    double tmp = z_x * z_x - z_y * z_y;
    z_y = 2 * z_x * z_y + c_y;
    z_x = tmp + c_x;

    iter++;
  }

  if (iter >= max_iter)
    return -1;
  else
    return iter;
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
void juliaset(double c_x, double c_y, double x0, double x1, int num_of_cols,
              double y0, double y1, int num_of_rows, int max_iter,
              int *image_matrix) {
  int idx = 0;
  for (int row = 0; row < num_of_rows; ++row) {
    for (int col = 0; col < num_of_cols; ++col) {
      double z_x = linspace(col, num_of_cols, x0, x1);
      double z_y = linspace(row, num_of_rows, y0, y1);

      image_matrix[idx++] = julia(z_x, z_y, c_x, c_y, max_iter);
    }
  }
}

int main(void) {
  const int width = 800;
  const int height = 800;
  const int maxiter = 10000;
  const char *file_name = "julia-fractal.pgm";
  const double cx = 0.2;
  const double cy = 0.55;

  std::vector<int> image(width * height);

  auto start = std::chrono::steady_clock::now();
  // Compute the fractal
  juliaset(cx, cy, -1.5, 1.5, width, -1.5, 1.5, height, maxiter, image.data());
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
             cx, cy, width, height);

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
