using Logging
using MOCNeutronTransport
using Test

# Turn off info, so we don't clutter up the test output
Logging.disable_logging(Logging.Info)
const Floats = [Float32, Float64, BigFloat]
include("setup/setup_geometry.jl")
#include("setup/setup_meshes.jl")
tests = ["geometry/Geometry",
#         "mesh/mesh",
        ]
for t in tests
  include("$(t).jl")
end
