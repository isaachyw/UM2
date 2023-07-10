include(${PROJECT_SOURCE_DIR}/cmake/add_um2_test.cmake)

# Test the framework itself
add_um2_test(test_framework test_framework.cpp)

# common
add_um2_test(bit ./common/bit.cpp)
add_um2_test(sto ./common/sto.cpp)
add_um2_test(vector ./common/vector.cpp)
add_um2_test(string ./common/string.cpp)
add_um2_test(color ./common/color.cpp)

# math
add_um2_test(morton ./math/morton.cpp)
add_um2_test(vec ./math/vec.cpp)
add_um2_test(mat ./math/mat.cpp)

# geometry
add_um2_test(point ./geometry/point.cpp)
add_um2_test(axis_aligned_box ./geometry/axis_aligned_box.cpp)
add_um2_test(line_segment ./geometry/line_segment.cpp)
add_um2_test(triangle ./geometry/triangle.cpp)

# ray_casting
add_um2_test(ray-line_segment ./ray_casting/intersect/ray-line_segment.cpp)
add_um2_test(ray-linear_polygon_mesh ./ray_casting/intersect/ray-linear_polygon_mesh.cpp)

# mesh
add_um2_test(face_vertex_mesh ./mesh/face_vertex_mesh.cpp)
add_um2_test(regular_grid ./mesh/regular_grid.cpp)
add_um2_test(regular_partition ./mesh/regular_partition.cpp)

# visualization
add_um2_test(image ./visualization/image.cpp)

if (UM2_ENABLE_VIS)
    add_um2_test(shader ./visualization/shader.cpp)
endif ()
