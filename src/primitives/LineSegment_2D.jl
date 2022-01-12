# A line segment in 2D space defined by its two endpoints.
struct LineSegment_2D{F <: AbstractFloat} <: Edge_2D{F}
    points::SVector{2, Point_2D{F}}
end

# Constructors
# -------------------------------------------------------------------------------------------------
LineSegment_2D(p₁::Point_2D, p₂::Point_2D) = LineSegment_2D(SVector(p₁, p₂))

# Methods
# -------------------------------------------------------------------------------------------------
# Interpolation
# l(0) yields points[1], and l(1) yields points[2]
@inline (l::LineSegment_2D{F})(r::Real) where {F <: AbstractFloat} = l[1] + (l[2] - l[1])F(r)
@inline arclength(l::LineSegment_2D) = distance(l[1], l[2])
@inline +(l::LineSegment_2D, p::Point_2D) = LineSegment_2D(l[1] + p, l[2] + p)

function intersect(l₁::LineSegment_2D{F}, l₂::LineSegment_2D{F}) where {F <: AbstractFloat}
    # NOTE: Doesn't work for colinear/parallel lines. (v⃗ × u⃗ = 0). Also, the cross product
    # operator for 2D points returns a scalar (the 2-norm of the cross product).
    #
    # Using the equation of a line in parametric form
    # For l₁ = x⃗₁ + rv⃗ and l₂ = x⃗₂ + su⃗
    # x⃗₁ + rv⃗ = x⃗₂ + su⃗                             subtracting x⃗₁ from both sides
    # rv⃗ = (x⃗₂-x⃗₁) + su⃗                             w⃗ = x⃗₂-x⃗₁
    # rv⃗ = w⃗ + su⃗                                   cross product with u⃗ (distributive)
    # r(v⃗ × u⃗) = w⃗ × u⃗ + s(u⃗ × u⃗)                   u⃗ × u⃗ = 0
    # r(v⃗ × u⃗) = w⃗ × u⃗                              dot product v⃗ × u⃗ to each side
    # r = (w⃗ × u⃗)/(v⃗ × u⃗)
    # Note that if the lines are parallel or collinear, v⃗ × u⃗ = 0
    # We need to ensure r, s ∈ [0, 1].
    # x⃗₂ + su⃗ = x⃗₁ + rv⃗                             subtracting x⃗₂ from both sides
    # su⃗ = -w⃗ + rv⃗                                  cross product with w⃗
    # s(u⃗ × w⃗) = -w⃗ × w⃗ + r(v⃗ × w⃗)                  w⃗ × w⃗ = 0 & substituting for r
    # s(u⃗ × w⃗) =  (v⃗ × w⃗)(w⃗ × u⃗)/(v⃗ × u⃗)            -(u⃗ × w⃗) = w⃗ × u⃗
    # s = -(v⃗ × w⃗)/(v⃗ × u⃗)                          -(v⃗ × w⃗) = w⃗ × v⃗
    # s = (w⃗ × v⃗)/(v⃗ × u⃗)
    #
    # Code is branchless, which is faster than failing early with 1e-8 < abs(wxu) or delaying division
    # by vxu and testing against r and s's numerators. This has been tested.
    ϵ = F(5e-6)
    v⃗ = l₁[2] - l₁[1]
    u⃗ = l₂[2] - l₂[1]
    w⃗ = l₂[1] - l₁[1]
    vxu = v⃗ × u⃗
    r = w⃗ × u⃗/vxu
    s = w⃗ × v⃗/vxu
    p = l₂(s)
    hit = 1e-8 < abs(vxu) && (-ϵ ≤ r ≤ 1 + ϵ) && (-ϵ ≤ s ≤ 1 + ϵ) 
    return (hit, p)
end

# Return if the point is left of the line segment
#   p    ^
#   ^   /
# v⃗ |  / u⃗
#   | /
#   o
#   We rely on v⃗ × u⃗ = |v⃗||u⃗|sin(θ). We may determine if θ ∈ (0, π] based on the sign of v⃗ × u⃗
@inline function isleft(p::Point_2D, l::LineSegment_2D)
    u⃗ = l[2] - l[1]
    v⃗ = p - l[1]
    return u⃗ × v⃗ >= 0
end

# A random line within [0, 1] × [0, 1]
function rand(::Type{LineSegment_2D{F}}) where {F <: AbstractFloat}
    return LineSegment_2D(rand(Point_2D{F}), rand(Point_2D{F}))
end

# N random lines within [0, 1] × [0, 1]
function rand(::Type{LineSegment_2D{F}}, N::Int64) where {F <: AbstractFloat}
    return [ LineSegment_2D(rand(Point_2D{F}), rand(Point_2D{F})) for i ∈ 1:N ]
end

# Plot
# -------------------------------------------------------------------------------------------------
if enable_visualization
    function convert_arguments(LS::Type{<:LineSegments}, l::LineSegment_2D)
        return convert_arguments(LS, [l[1], l[2]])
    end

    function convert_arguments(LS::Type{<:LineSegments}, L::Vector{<:LineSegment_2D})
        return convert_arguments(LS, reduce(vcat, [[l[1], l[2]] for l in L]))
    end
end
