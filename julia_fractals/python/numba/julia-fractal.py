#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

from numba import njit


@njit
def julia(z, c, maxiter=256):
    """Determine if point "z" belongs to the Julia set J_c

    Return -1 if the point (start_x) + (start_y)*i belongs to the
    Julia set parametrized by (c_x) + (c_y)*i. Otherwise, return the
    number of iterations needed to find that the limit diverges. Do
    not iterate more than "max_iter" times, so that the return value
    of this function is always in the range [-1, max_iter].

    """

    iteridx = 0
    while (z.real ** 2 + z.imag ** 2 < 4) and (iteridx < maxiter):
        # Python supports complex numbers
        z = z * z + c
        iteridx += 1

    if iteridx == maxiter:
        return -1
    else:
        return iteridx


def linspace(val, max, out_min, out_max):
    """Apply a linear transformation to "val".

    Apply a linear transformation to "val", so that the following
    applies:

    - If "val" == 0, then "out_min" is returned;

    - If "val" == max - 1, then "out_max" is returned;

    - For any other value, return a linear interpolation between the
      two extrema

    """
    return out_min + (val * (out_max - out_min)) / (max - 1)


def juliaset(c, xlim, ylim, num_of_rows, num_of_cols, maxiter):
    """Apply the function "julia" to a matrix of points.

    The matrix of points corresponds to the subdomain [x0, x1] x [y0,
     y1] of the complex plane. The size of the matrix is
     num_of_cols*num_of_rows, and it is saved in the array
     "image_matrix" (row-first order).
    """

    # The syntax "*xlim" expands the tuple "xlim" (containing two
    # elements), so that the call below is the same as the following:
    #
    # complex(
    #     real=linspace(col, num_of_cols, xlim[0], xlim[1]),
    #     imag=linspace(row, num_of_rows, ylim[0], ylim[1]),
    # )
    #
    # assuming that both xlim and ylim have two elements (as in this
    # case).

    list_of_z = [
        complex(
            real=linspace(col, num_of_cols, *xlim),
            imag=linspace(row, num_of_rows, *ylim),
        )
        for row in range(num_of_rows)
        for col in range(num_of_cols)
    ]
    return [julia(z, c, maxiter) for z in list_of_z]


def main():
    import time
    import math

    width, height = 800, 800
    maxiter = 10000
    file_name = "julia-fractal.pgm"
    c = 0.2 + 0.55j

    start = time.time()
    image = juliaset(
        c=c,
        xlim=(-1.5, 1.5),
        ylim=(-1.5, 1.5),
        num_of_rows=height,
        num_of_cols=width,
        maxiter=maxiter,
    )
    end = time.time()
    print(f"Time required to compute the fractal: {(end - start) * 1000.0} ms")

    max_value = max(image)

    with open(file_name, "wt") as output_file:
        output_file.write(
            f"""P2
# Julia set for the point {c}
{width} {height}
255
"""
        )

        for pixel in image:
            if pixel < 0:
                val = 0.0
            else:
                val = math.pow(1.0 - pixel / max_value, 5.0)

            output_file.write("{} ".format(int(val * 255 + 0.5)))

    print(f'Image written to file "{file_name}"')


main()
