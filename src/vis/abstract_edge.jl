# LineSegment
function convert_arguments(LS::Type{<:LineSegments}, l::LineSegment)
    return convert_arguments(LS, vertices(l))
end

function convert_arguments(
        LS::Type{<:LineSegments},
        lines::Vector{LineSegment{D, T}}) where {D, T}
    points = Vector{NTuple{D, T}}(undef, 2*length(lines))
    for i in eachindex(lines)
        points[2 * i - 1] = coord(lines[i][1])
        points[2 * i    ] = coord(lines[i][2])
    end
    return convert_arguments(LS, points)
end

# QuadraticSegment
function convert_arguments(LS::Type{<:LineSegments}, q::QuadraticSegment) where {T}
    return convert_arguments(
            LS, 
            [ q(r) for r in LinRange(0, 1, UM2_VIS_NONLINEAR_SUBDIVISIONS + 1) ]
           )
end

function convert_arguments(
        LS::Type{<:LineSegments}, 
        Q::Vector{QuadraticSegment{D, T}}) where {D, T}
    N = length(Q)
    NDIV = UM2_VIS_NONLINEAR_SUBDIVISIONS
    points = Vector{NTuple{D, T}}(undef, 2*N*NDIV)
    rr = LinRange{T}(0, 1, NDIV + 1)
    for i = 1:N
        q = Q[i]
        p = coord(q(rr[1]))
        points[2 * (i - 1) * NDIV + 1] = p
        for j in 2:NDIV
            p = coord(q(rr[j]))
            points[2 * (i - 1) * NDIV + 2 * (j - 1)    ] = p
            points[2 * (i - 1) * NDIV + 2 * (j - 1) + 1] = p
        end
        p = coord(q(rr[NDIV + 1]))
        points[2 * (i - 1) * NDIV + 2 * NDIV     ] = p
    end
    return convert_arguments(LS, points)
end
