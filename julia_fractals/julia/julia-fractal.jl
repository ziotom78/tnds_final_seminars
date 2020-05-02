using Images
using Printf

function julia(start, c, max_iter)
    iter = 1
    z = start
    while (abs2(z) < 4.0) && (iter <= max_iter)
        z = z * z + c
        iter += 1
    end

    iter > max_iter ? -1 : iter
end

function juliaset(c, xrange, yrange, maxiter)
    m = Array{Float64}(undef, (length(xrange), length(yrange)))

    @inbounds for j in 1:length(yrange)
        y = yrange[j]
        for i in 1:length(xrange)
            x = xrange[i]
            val = julia(x + y * im, c, maxiter)
            if val < 0
                m[i, j] = 0
            else
                m[i, j] = val / maxiter
            end
        end
    end

    m
end

function main()
    filename = "julia-fractal.png"

    println("Burn-in phase...")
    for i = 1:3
        juliaset(
            0.2 + 0.55im,
            range(-2.0, 2.0, length=640),
            range(-2.0, 2.0, length=640),
            10_000,
        )
    end

    println("Running the calculation...")
    @time m = juliaset(
        0.2 + 0.55im,
        range(-2.0, 2.0, length=800),
        range(-2.0, 2.0, length=800),
        10_000,
    )

    maxval = maximum(m)
    if maxval > 0
        m ./= maximum(m)
    else
        println("Warning: black image")
    end

    save(filename, Gray.(m.^0.5))
    @printf("File \"%s\" created successfully", filename)
end

main()
