# A 2D point in Cartesian coordinates.
struct Point_2D{F <: AbstractFloat} <: FieldVector{2, F}
    x::F
    y::F
end

# Constructors
# -------------------------------------------------------------------------------------------------
# 1D constructor
Point_2D(x::F) where {F <: AbstractFloat} = Point_2D(x, F(0))

# Single conversion constructor
Point_2D(::Type{F}, x::X, y::Y) where {F <: AbstractFloat,
                                       X <: Real,
                                       Y <: Real} = Point_2D(F(x), F(y))
# 1D single conversion constructor
Point_2D(::Type{F}, x::X) where {F <: AbstractFloat,
                                 X <: Real} = Point_2D(F(x), F(0))

# Operators
# -------------------------------------------------------------------------------------------------
≈(p₁::Point_2D, p₂::Point_2D) = distance(p₁, p₂) < Point_2D_differentiation_distance
≉(p₁::Point_2D, p₂::Point_2D) = !(p₁ ≈ p₂)
# Note the cross product of two 2D points returns a scalar. It is assumed that this is the
# desired quantity, since the cross product of vectors in the plane is a vector normal to the plane.
# Hence the z coordinate of the resultant vector is returned.
×(p₁::Point_2D, p₂::Point_2D) = p₁.x*p₂.y - p₂.x*p₁.y
⋅(p₁::Point_2D, p₂::Point_2D) = p₁.x*p₂.x + p₁.y*p₂.y
+(p::Point_2D, n::Real) = Point_2D(p.x + n, p.y + n)
+(n::Real, p::Point_2D) = p + n
-(p::Point_2D, n::Real) = Point_2D(p.x - n, p.y - n)
-(n::Real, p::Point_2D) = p - n
*(n::Real, p::Point_2D) = Point_2D(n*p.x, n*p.y)
*(p::Point_2D, n::Real) = n*p
/(p::Point_2D, n::Real) = Point_2D(p.x/n, p.y/n)
-(p::Point_2D) = -1*p

# Methods
# -------------------------------------------------------------------------------------------------
# note: hypot is the Julia recommended way to do sqrt of sum squared for 2 numbers
# @code_warntype checked 2021/11/19
norm(p::Point_2D) = hypot(p.x, p.y)
distance(p₁::Point_2D, p₂::Point_2D) = norm(p₁ - p₂)
midpoint(p₁::Point_2D, p₂::Point_2D) = (p₁ + p₂)/2

# Plot
# -------------------------------------------------------------------------------------------------
if enable_visualization
    convert_arguments(S::Type{<:Scatter}, p::Point_2D) = convert_arguments(S, p.x)
    function convert_arguments(S::Type{<:Scatter}, P::Vector{<:Point_2D})
        return convert_arguments(S, [p.x for p in P])
    end
end
