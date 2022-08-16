export AABox,
       AABox2,
       AABox2f,
       AABox2d,
       AABB,
       AABB2,
       AABB2f,
       AABB2d

export minima, maxima,
       x_min, y_min, z_min,
       x_max, y_max, z_max,
       Δx, Δy, Δz,
       area,
       bounding_box

# AXIS-ALIGNED BOX
# -----------------------------------------------------------------------------
#
# An axis-aligned box is a hyperrectangle with axis-aligned faces and edges.
# It is defined by its minima and maxima (corners closest and furthest from
# the origin).
#

struct AABox{D, T <: AbstractFloat}
    minima::Point{D, T}
    maxima::Point{D, T}
end

# -- Type aliases --

const AABox2 = AABox{2}
const AABox2f = AABox{2, Float32}
const AABox2d = AABox{2, Float64}
const AABB = AABox
const AABB2 = AABox2
const AABB2f = AABox2f
const AABB2d = AABox2d

# -- Accessors --

minima(aab::AABox) = aab.minima
maxima(aab::AABox) = aab.maxima

x_min(aab::AABox) = aab.minima[1]
y_min(aab::AABox) = aab.minima[2]
z_min(aab::AABox{3}) = aab.minima[3]
x_max(aab::AABox) = aab.maxima[1]
y_max(aab::AABox) = aab.maxima[2]
z_max(aab::AABox{3}) = aab.maxima[3]

Δx(aab::AABox) = x_max(aab) - x_min(aab)
Δy(aab::AABox) = y_max(aab) - y_min(aab)
Δz(aab::AABox{3}) = z_max(aab) - z_min(aab)

# -- Measure --

area(aab::AABox{2}) = prod(maxima(aab) - minima(aab))

# -- In --

function Base.in(p::Point{2}, aab::AABox{2})
    return x_min(aab) ≤ p[1] ≤ x_max(aab) &&
           y_min(aab) ≤ p[2] ≤ y_max(aab)
end

# -- Miscellaneous --

function Base.isapprox(aab₁::AABox, aab₂::AABox)
    return minima(aab₁) ≈ minima(aab₂) &&
           maxima(aab₁) ≈ maxima(aab₂)
end

function Base.union(aab₁::AABox{2}, aab₂::AABox{2})
    xmin = min(x_min(aab₁), x_min(aab₂))
    xmax = max(x_max(aab₁), x_max(aab₂))
    ymin = min(y_min(aab₁), y_min(aab₂))
    ymax = max(y_max(aab₁), y_max(aab₂))
    return AABox(Point(xmin, ymin), Point(xmax, ymax))
end

# -- Bounding box --

# Vector of points
function bounding_box(points::Vector{Point{2, T}}) where {T}
    xmin = ymin = typemax(T)
    xmax = ymax = typemin(T)
    for i in 1:length(points)
        x, y = points[i]
        xmin = min(xmin, x)
        ymin = min(ymin, y)
        xmax = max(xmax, x)
        ymax = max(ymax, y)
    end
    return AABox{2, T}(Point{2, T}(xmin, ymin), Point{2, T}(xmax, ymax))
end

# Vec of points
function bounding_box(points::Vec{L, Point{2, T}}) where {L, T}
    xmin = ymin = typemax(T)
    xmax = ymax = typemin(T)
    for i in 1:L
        x, y = points[i]
        xmin = min(xmin, x)
        ymin = min(ymin, y)
        xmax = max(xmax, x)
        ymax = max(ymax, y)
    end
    return AABox{2, T}(Point{2, T}(xmin, ymin), Point{2, T}(xmax, ymax))
end

# -- IO --

function Base.show(io::IO, aab::AABox{D, T}) where {D, T}
    print(io, "AABox", D)
    if T === Float32
        print(io, 'f')
    elseif T === Float64
        print(io, 'd')
    else
        print(io, '?')
    end
    return print(io, "(", minima(aab), maxima(aab), ")")
end
