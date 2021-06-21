struct Triangle6_2D{T <: AbstractFloat}
    points::NTuple{6, Point_2D{T}}
end

# Constructors
# -------------------------------------------------------------------------------------------------
Triangle6_2D(p₁::Point_2D{T}, 
         p₂::Point_2D{T}, 
         p₃::Point_2D{T},
         p₄::Point_2D{T},
         p₅::Point_2D{T},
         p₆::Point_2D{T}
        ) where {T <: AbstractFloat} = Triangle6_2D((p₁, p₂, p₃, p₄, p₅, p₆))


# Methods
# -------------------------------------------------------------------------------------------------
function (tri6::Triangle6_2D{T})(r::R, s::S) where {T <: AbstractFloat, R,S <: Real}
    r_T = T(r)
    s_T = T(s)
    return (1 - r_T - s_T)*(2(1 - r_T - s_T) - 1)*tri6.points[1] +
                                     r_T*(2r_T-1)*tri6.points[2] +
                                     s_T*(2s_T-1)*tri6.points[3] +
                             4r_T*(1 - r_T - s_T)*tri6.points[4] +
                                       (4r_T*s_T)*tri6.points[5] +
                             4s_T*(1 - r_T - s_T)*tri6.points[6]
end

function derivatives(tri6::Triangle6_2D{T}, r::R, s::S) where {T <: AbstractFloat, R,S <: Real}
    # Return ( ∂tri6/∂r, ∂tri6/∂s )
    r_T = T(r)
    s_T = T(s)
    ∂T_∂r = (4r_T + 4s_T - 3)*tri6.points[1] +
                   (4r_T - 1)*tri6.points[2] +
            4(1 - 2r_T - s_T)*tri6.points[4] +
                       (4s_T)*tri6.points[5] +
                      (-4s_T)*tri6.points[6]

    ∂T_∂s = (4r_T + 4s_T - 3)*tri6.points[1] +
                   (4s_T - 1)*tri6.points[3] +
                      (-4r_T)*tri6.points[4] +
                       (4r_T)*tri6.points[5] +
            4(1 - r_T - 2s_T)*tri6.points[6]
    return (∂T_∂r, ∂T_∂s)
end

function jacobian(tri6::Triangle6_2D{T}, r::R, s::S) where {T <: AbstractFloat, R,S <: Real}
    (∂T_∂r, ∂T_∂s) = derivatives(tri6, r, s) 
    return hcat(∂T_∂r.x, ∂T_∂s.x)
end

function area(tri6::Triangle6_2D{T}; N::Int64=12) where {T <: AbstractFloat}
    # Numerical integration required. Gauss-Legendre quadrature over a triangle is used.
    # Let T(r,s) be the interpolation function for tri6,
    #                             1 1-r                          N
    # A = ∬ ||∂T/∂r × ∂T/∂s||dA = ∫  ∫ ||∂T/∂r × ∂T/∂s|| ds dr = ∑ wᵢ||∂T/∂r(rᵢ,sᵢ) × ∂T/∂s(rᵢ,sᵢ)||
    #      D                      0  0                          i=1
    w, r, s = gauss_legendre_quadrature(tri6, N)
    a = T(0)
    for i in 1:N
        (∂T_∂r, ∂T_∂s) = derivatives(tri6, r[i], s[i])
        a += w[i] * abs(∂T_∂r × ∂T_∂s)
    end
    return a
end

function triangulate(tri6::Triangle6_2D{T}, N::Int64) where {T <: AbstractFloat}
    triangles = Vector{Triangle_2D{T}}(undef, (N+1)*(N+1))
    if N == 0
        triangles[1] = Triangle_2D(tri6.points[1], tri6.points[2], tri6.points[3])
    else
        i = 1
        for S = 1:N, R = 0:N-S
            triangles[i] = Triangle_2D(tri6(    R/(N+1),     S/(N+1)),
                                       tri6((R+1)/(N+1),     S/(N+1)),
                                       tri6(    R/(N+1), (S+1)/(N+1)))
            triangles[i+1] = Triangle_2D(tri6(    R/(N+1),     S/(N+1)),
                                         tri6((R+1)/(N+1), (S-1)/(N+1)),
                                         tri6((R+1)/(N+1),     S/(N+1)))
            i += 2
        end
        j = (N+1)*N + 1
        for S = 0:0, R = 0:N-S
            triangles[j] = Triangle_2D(tri6(    R/(N+1),     S/(N+1)),
                                       tri6((R+1)/(N+1),     S/(N+1)),
                                       tri6(    R/(N+1), (S+1)/(N+1)))
            j += 1
        end
    end
    return triangles 
end

function in(p::Point_2D{T}, tri6::Triangle6_2D{T}; N::Int64=6) where {T <: AbstractFloat}
    r = T(1//3)
    s = T(1//3)
    # 6 iterations appears to be sufficient for all cases.
    # Inverstion of a 2 by 2 matrix is so fast, it doesn't make sense to check the norm of the error
    # and exit conditionally.
    # Note the number of iterations here is less than real_to_parametric since if the point really is inside
    # the triangle, then the convergence is very quick.
    for i = 1:N
        err = p - tri6(r, s)
        # Inversion is faster for 2 by 2 than \
        Δr, Δs = inv(jacobian(tri6, r, s)) * err.x
        r = r + Δr
        s = s + Δs
    end
    err = p - tri6(r, s)
    if (-1.0e-5 ≤ r ≤ 1.00001) && (-1.0e-5 ≤ s ≤ 1.00001) && norm(err) < 1.0e-5  
        return true
    else
        return false
    end
end

function real_to_parametric(p::Point_2D{T}, tri6::Triangle6_2D{T}; N::Int64=10) where {T <: AbstractFloat}
    r = T(1//3)
    s = T(1//3)
    # 10 iterations appears to be sufficient for all realistic use cases, even 100 units away.
    # Inverstion of a 2 by 2 matrix is so fast, it doesn't make sense to check the norm of the error
    # and exit conditionally.
    for i = 1:N
        err = p - tri6(r, s)
        # Inversion is faster for 2 by 2 than \
        Δr, Δs = inv(jacobian(tri6, r, s)) * err.x
        r = r + Δr
        s = s + Δs
    end
    return Point_2D(r, s)
end

# Plot
# -------------------------------------------------------------------------------------------------
function convert_arguments(P::Type{<:LineSegments}, tri6::Triangle6_2D{T}) where {T <: AbstractFloat}
    q₁ = QuadraticSegment_2D(tri6.points[1], tri6.points[2], tri6.points[4])
    q₂ = QuadraticSegment_2D(tri6.points[2], tri6.points[3], tri6.points[5])
    q₃ = QuadraticSegment_2D(tri6.points[3], tri6.points[1], tri6.points[6])
    qsegs = [q₁, q₂, q₃]
    return convert_arguments(P, qsegs)
end

function convert_arguments(P::Type{<:LineSegments}, 
        TA::AbstractArray{<:Triangle6_2D{T}}) where {T <: AbstractFloat}
    point_sets = [convert_arguments(P, tri6) for tri6 in TA]
    return convert_arguments(P, reduce(vcat, [pset[1] for pset in point_sets]))
end

function convert_arguments(P::Type{Mesh{Tuple{Triangle6_2D{T}}}}, 
        tri6::Triangle6_2D{T}) where {T <: AbstractFloat}
    triangles = triangulate(tri6, 13)
    return convert_arguments(P, triangles)
end

function convert_arguments(MT::Type{Mesh{Tuple{Triangle6_2D{T}}}},
        AT::Vector{Triangle_2D{T}}) where {T <: AbstractFloat}
    points = reduce(vcat, [[tri.points[i].x for i = 1:3] for tri in AT])
    faces = zeros(Int64, length(AT), 3)
    k = 1
    for i in 1:length(AT), j = 1:3
        faces[i, j] = k
        k += 1
    end
    return convert_arguments(MT, points, faces)
end
