export Polytope, Edge, LineSegment, QuadraticSegment, Face, Polygon, QuadraticPolygon,
       Triangle, Quadrilateral, QuadraticTriangle, QuadraticQuadrilateral, Cell,
       Polyhedron, QuadraticPolyhedron, Tetrahedron, Hexahedron, QuadraticTetrahedron,
       QuadraticHexahedron
export ==, ridges, facets, alias_string

"""

    Polytope{K,P,N,T}

A `K`-polytope of order `P` with `N` vertices of type `T`.

## Aliases

```julia
# 1-polytope
const Edge                      = Polytope{1}
const LineSegment               = Edge{1,2}
const QuadraticSegment          = Edge{2,3}
# 2-polytope
const Face                      = Polytope{2}
const Polygon                   = Face{1}
const QuadraticPolygon          = Face{2}
const Triangle                  = Polygon{3}
const Quadrilateral             = Polygon{4}
const QuadraticTriangle         = QuadraticPolygon{6}
const QuadraticQuadrilateral    = QuadraticPolygon{8}
# 3-polytope
const Cell                      = Polytope{3}
const Polyhedron                = Cell{1}
const QuadraticPolyhedron       = Cell{2}
const Tetrahedron               = Polyhedron{4}
const Hexahedron                = Polyhedron{8}
const QuadraticTetrahedron      = QuadraticPolyhedron{10}
const QuadraticHexahedron       = QuadraticPolyhedron{20}
```
### Notes
- These are Lagrangian finite elements.
- This struct only supports the shapes found in "The Visualization Toolkit:
  An Object-Oriented Approach to 3D Graphics, 4th Edition, Chapter 8, Advanced
  Data Representation".
- See the VTK book for specific vertex ordering info, but generally vertices are
  ordered in a counterclockwise fashion, with vertices of the linear shape given
  first.
- See https://en.wikipedia.org/wiki/Polytope for help with terminology.
"""
struct Polytope{K,P,N,T}
    vertices::Vec{N,T}
    Polytope{K,P,N,T}(vertices::Vec{N,T}) where {K,P,N,T} = new{K,P,N,T}(vertices)
end

# type aliases
# 1-polytope
const Edge                      = Polytope{1}
const LineSegment               = Edge{1,2}
const QuadraticSegment          = Edge{2,3}
# 2-polytope
const Face                      = Polytope{2}
const Polygon                   = Face{1}
const QuadraticPolygon          = Face{2}
const Triangle                  = Polygon{3}
const Quadrilateral             = Polygon{4}
const QuadraticTriangle         = QuadraticPolygon{6}
const QuadraticQuadrilateral    = QuadraticPolygon{8}
# 3-polytope
const Cell                      = Polytope{3}
const Polyhedron                = Cell{1}
const QuadraticPolyhedron       = Cell{2}
const Tetrahedron               = Polyhedron{4}
const Hexahedron                = Polyhedron{8}
const QuadraticTetrahedron      = QuadraticPolyhedron{10}
const QuadraticHexahedron       = QuadraticPolyhedron{20}

# constructors
Polytope{K,P,N,T}(vertices...) where {K,P,N,T} = Polytope{K,P,N,T}(Vec{N,T}(vertices))
Polytope{K,P,N}(vertices::Vec{N,T}) where {K,P,N,T} = Polytope{K,P,N,T}(vertices)
Polytope{K,P,N}(vertices...) where {K,P,N} = Polytope{K,P,N}(Vec(vertices))

Base.getindex(poly::Polytope, i::Int) = Base.getindex(poly.vertices, i)

facets(p::Polytope{1}) = p.vertices
facets(p::Polytope{2}) = edges(p)
# facets(p::Polytope{3}) = faces(p)
#
ridges(p::Polytope{2}) = p.vertices
#ridges(p::Polytope{3}) = # unique edges of the faces

function alias_string(::Type{P}) where {P<:Polytope}
    P <: LineSegment            && return "LineSegment"
    P <: QuadraticSegment       && return "QuadraticSegment"
    P <: Triangle               && return "Triangle"
    P <: Quadrilateral          && return "Quadrilateral"
    P <: QuadraticTriangle      && return "QuadraticTriangle"
    P <: QuadraticQuadrilateral && return "QuadraticQuadrilateral"
    P <: Tetrahedron            && return "Tetrahedron"
    P <: Hexahedron             && return "Hexahedron"
    P <: QuadraticTetrahedron   && return "QuadraticTetrahedron"
    P <: QuadraticHexahedron    && return "QuadraticHexahedron"
    # fallback on default
    return "$(P)"
end

# If we think of the polytopes as set, P₁ ∩ P₂ = P₁ and P₁ ∩ P₂ = P₂ implies P₁ = P₂
# Can we say sort(P₁.vertices) == sort(P₂.vertices) is sufficient?
function ==(P₁::LineSegment{T}, P₂::LineSegment{T}) where {T}
    return (P₁[1] == P₂[1] && P₁[2] == P₂[2]) || (P₁[1] == P₂[2] && P₁[2] == P₂[1])
end

function ==(P₁::QuadraticSegment{T}, P₂::QuadraticSegment{T}) where {T}
    return ((P₁[1] == P₂[1] && P₁[2] == P₂[2])  || 
            (P₁[1] == P₂[2] && P₁[2] == P₂[1])
           ) && P₁[3] == P₂[3]
end

# Show aliases when printing
function Base.show(io::IO, poly::Polytope)
    print(io, alias_string(typeof(poly)),"(",poly.vertices, ")")
end
