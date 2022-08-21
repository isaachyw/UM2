module UM2

const UM2_ENABLE_VIS = true

include("common/types.jl")
include("common/constants.jl")

include("math/vec.jl")
include("math/mat.jl")

include("geometry/point.jl")
include("geometry/axis_aligned_box.jl")
include("geometry/line_segment.jl")
include("geometry/quadratic_segment.jl")
include("geometry/triangle.jl")
include("geometry/quadrilateral.jl")
include("geometry/quadratic_triangle.jl")
include("geometry/quadratic_quadrilateral.jl")

include("mesh/tri_mesh.jl")

include("ray_casting/ray.jl")
include("ray_casting/angular_quadrature.jl")
include("ray_casting/modular_rays.jl")

if UM2_ENABLE_VIS
    include("vis/setup.jl")
    include("vis/point.jl")
    include("vis/axis_aligned_box.jl")
    include("vis/line_segment.jl")
    include("vis/quadratic_segment.jl")
    include("vis/triangle.jl")
    include("vis/quadrilateral.jl")
    include("vis/quadratic_triangle.jl")
    include("vis/quadratic_quadrilateral.jl")
    include("vis/ray.jl")
end
#include("quadrature/Quadrature.jl")
#include("raytracing/Raytracing.jl")
#include("physics/Physics.jl")
#include("mpact/MPACT.jl")
#include("gmsh/Gmsh.jl")
end
