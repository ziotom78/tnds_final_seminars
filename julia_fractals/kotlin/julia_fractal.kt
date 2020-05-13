/* To run and compile this example, you need the Kotlin compiler
 *
 * # Compiling
 * kotlinc julia_fractal.kt -include-runtime -d julia_fractal.jar
 *
 * # Running
 * java -jar julia_fractal.jar
 *
 * There are Kotlin libraries to natively use complex numbers. We'll
 * avoid using these, because it requires using advanced build tools
 * like Gradle.
 */

import java.io.File
import java.awt.Color
import java.awt.image.BufferedImage
import java.awt.image.BufferedImage.TYPE_BYTE_GRAY
import javax.imageio.ImageIO

import kotlin.system.measureTimeMillis

fun julia(start_x: Double, start_y: Double, c_x: Double, c_y: Double, max_iter: Int): Int {
    var iter = 0
    var z_x = start_x
    var z_y = start_y

    while((z_x * z_x + z_y * z_y < 4.0) && (iter < max_iter)) {
        val tmp = z_x * z_x - z_y * z_y
        z_y = 2 * z_x * z_y + c_y
        z_x = tmp + c_x

        iter++
    }

    return if (iter >= max_iter) -1 else iter
}

fun linspace(value: Int, max: Int, out_min: Double, out_max: Double): Double {
    return out_min + (value * (out_max - out_min)) / (max - 1)
}

fun juliaset(c_x: Double, c_y: Double,
             x0: Double, x1: Double, num_of_cols: Int,
             y0: Double, y1: Double, num_of_rows: Int,
             max_iter: Int, image: IntArray)
{
    var idx = 0
    for (row: Int in 0..num_of_rows - 1) {
        for (col: Int in 0..num_of_cols - 1) {
            val z_x = linspace(col, num_of_cols, x0, x1)
            val z_y = linspace(row, num_of_rows, y0, y1)

            image[idx] = julia(z_x, z_y, c_x, c_y, max_iter)
            idx++
        }
    }
}

fun main() {
    val width = 800
    val height = 800
    val maxiter = 10000
    val file_name = "julia-fractal.png"
    val cx = 0.2
    val cy = 0.55

    var image = IntArray(width * height)

    val time = measureTimeMillis {
        juliaset(cx, cy, -1.5, 1.5, width, -1.5, 1.5, height, maxiter, image)
    }

    println("Time required to compute the fractal: $time ms")

    val max_value = image.max()!!
    
    var buffered_image = BufferedImage(width, height, TYPE_BYTE_GRAY)
    for (j: Int in 0..height - 1) {
        for (i: Int in 0..width - 1) {
            val cur_iter = image[j * height + i]
            val component = if (cur_iter < 0) {
                0.0
            } else {
                (Math.pow(1.0 - cur_iter.toDouble() / max_value, 5.0) * 255.0)
            }

            val int_component = component.toInt()
            val color = Color(int_component, int_component, int_component)
            buffered_image.setRGB(i, j, color.rgb)
        }

    }

    var output_file = File(file_name)
    ImageIO.write(buffered_image, "png", output_file)
}
