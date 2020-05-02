# If you're using GCC, compile with this command:
#
# nim c --run -d:release --passC:-flto julia_fractals.nim
#
# (be sure to have a recent version of the GCC compiler,
# see https://forum.nim-lang.org/t/6283)

import complex
import math
import times
import strformat

func julia(z: Complex64, c: Complex64, maxiter: int = 256): int =
    var iteridx = 0
    var cur_z = z
    while (abs2(cur_z) < 4) and (iteridx < maxiter):
      cur_z = cur_z * cur_z + c
      
      iteridx += 1

    if iteridx == maxiter:
        return -1
    else:
        return iteridx


func linspace(val: int, max: int, out_min: float, out_max: float): float {.inline.} =
    return out_min + (float(val) * (out_max - out_min)) / float(max - 1)


func juliaset(c: Complex64,
              x_min: float, x_max: float,
              y_min: float, y_max: float,
              num_of_rows: int, num_of_cols: int,
              maxiter: int): seq[int] =

    result = newSeq[int](num_of_rows * num_of_cols)
    
    var idx = 0
    for row in 1..num_of_rows:
      for col in 1..num_of_cols:
        let z = complex64(re = linspace(col, num_of_cols, x_min, x_max),
                          im = linspace(row, num_of_rows, y_min, y_max))

        result[idx] = julia(z, c, maxiter)
        idx += 1


proc main() =
  const (width, height) = (800, 800)
  const maxiter = 10000
  const file_name = "julia-fractal.pgm"
  const c = complex64(re = 0.2, im = 0.55)

  let start = now()
  let image = juliaset(
    c = c,
    x_min = -1.5, x_max = 1.5,
    y_min = -1.5, y_max = 1.5,
    num_of_rows = height,
    num_of_cols = width,
    maxiter = maxiter,
  )
  let stop = now()
  let duration = (stop - start).inMilliseconds
  echo fmt"Time required to compute the fractal: {duration} ms"

  let max_value = max(image)

  var output_file = open(file_name, fmWrite)
  write(output_file, fmt"""P2
# Julia set for the point {c}
{width} {height}
255
""")

  for pixel in image:
    let val = if pixel < 0:
      0.0
    else:
      pow(1.0 - pixel / max_value, 5.0)
      
    write(output_file, int(val * 255 + 0.5), " ")

  echo fmt"Image written to file {file_name}"

main()
