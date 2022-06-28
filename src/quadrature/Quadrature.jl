export AbstractRefShape, RefSimplex, RefHypercube, RefLine, RefTriangle, RefTetrahedron,
       RefSquare, RefCube

abstract type AbstractRefShape end
struct RefSimplex{K} <: AbstractRefShape end
struct RefHypercube{K} <: AbstractRefShape end

# RefLine could be RefSimplex or RefHypercube. We use RefHypercube here for simplicity.
const RefTriangle    = RefSimplex{2}
# const RefTetrahedron = RefSimplex{3}

const RefLine   = RefHypercube{1}
const RefSquare = RefHypercube{2}
const RefCube   = RefHypercube{3}

include("gauss_quadrature.jl")
include("angular_quadrature.jl")
