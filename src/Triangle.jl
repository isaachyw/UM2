import Base: intersect, in
import GLMakie: convert_arguments, LineSegments

struct Triangle{T <: AbstractFloat} <: Face
    points::NTuple{3, Point{T}}
end

# Constructors
# -------------------------------------------------------------------------------------------------
Triangle(p₁::Point{T}, 
         p₂::Point{T}, 
         p₃::Point{T}) where {T <: AbstractFloat} = Triangle((p₁, p₂, p₃))

# Methods
# -------------------------------------------------------------------------------------------------
# Evaluation in Barycentric coordinates
function (tri::Triangle)(r::T, s::T) where {T <: AbstractFloat}
    return (1 - r - s)*tri.points[1] + r*tri.points[2] + s*tri.points[3]
end

function area(tri::Triangle{T}) where {T <: AbstractFloat}
    # A = bh/2
    # Let u⃗ = |v₂ - v₁|, v⃗ = |v₃ - v₁|
    # b = |u⃗|
    # h = |sin(θ) v⃗|, where θ is the angle between u⃗ and v⃗
    # u⃗ × v⃗ = |u⃗||v⃗| sin(θ), hence
    # A = |u⃗ × v⃗|/2 = bh/2
    u⃗ = tri.points[2] - tri.points[1] 
    v⃗ = tri.points[3] - tri.points[1] 
    return norm(u⃗ × v⃗)/2
end

function intersect(l::LineSegment{type}, tri::Triangle{type}) where {type <: AbstractFloat}
    # Algorithm is from
    # Möller, T., & Trumbore, B. (1997). Fast, minimum storage ray-triangle intersection. 
    p = zero(l.points[1])

    E₁ = tri.points[2] - tri.points[1]
    E₂ = tri.points[3] - tri.points[1]
    T = l.points[1] - tri.points[1]
    D = l.points[2] - l.points[1]
    P = D × E₂   
    Q = T × E₁
    det = P ⋅ E₁
    if isapprox(det, 0, atol = sqrt(eps(type))) 
        return false, p
    else
        u = (P ⋅ T)/det
        v = (Q ⋅ D)/det
        t = (Q ⋅ E₂)/det
        return (u < 0) || (v < 0) || (u + v > 1) || (t < 0) || (1 < t) ? (false, p) : (true, l(t))
    end
end

function in(p::Point{T}, tri::Triangle{T}) where {T <: AbstractFloat}
    # If the volume of the tetrahedron formed by the 4 points is 0, then point lies in the
    # plane of the triangle. Division by 6 is dropped, since it doesn't change the computation.
    u⃗ = tri.points[2] - tri.points[1]   
    v⃗ = tri.points[3] - tri.points[1]    
    w⃗ = p - tri.points[1]
    V = norm(u⃗ ⋅ (v⃗ × w⃗))
    if isapprox(V, 0, atol=sqrt(eps(T)))
        # If the point is within the plane of the triangle, then the point is only within the triangle
        # if the areas of the triangles formed by the point and each pair of two vertices sum to the 
        # area of the triangle. Division by 2 is dropped, since it cancels
        A₁ = norm((tri.points[1] - p) × (tri.points[2] - p))
        A₂ = norm((tri.points[2] - p) × (tri.points[3] - p))
        A₃ = norm((tri.points[3] - p) × (tri.points[1] - p))
        A  = norm(u⃗ × v⃗)
        return A₁ + A₂ + A₃ ≈ A
    else
        return false
    end
end

# Plot
# -------------------------------------------------------------------------------------------------
function convert_arguments(P::Type{<:LineSegments}, tri::Triangle)
    return convert_arguments(P, [tri.points[1].coord, 
                                 tri.points[2].coord,
                                 tri.points[2].coord, 
                                 tri.points[3].coord,
                                 tri.points[3].coord, 
                                 tri.points[1].coord,
                                ])
end

function convert_arguments(P::Type{<:LineSegments}, AT::AbstractArray{<:Triangle})
    return convert_arguments(P, reduce(vcat, [
                                [tri.points[1].coord, 
                                 tri.points[2].coord,
                                 tri.points[2].coord, 
                                 tri.points[3].coord,
                                 tri.points[3].coord, 
                                 tri.points[1].coord] for tri in AT]))
end
