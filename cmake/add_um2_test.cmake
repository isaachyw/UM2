set(TEST_FRAMEWORK_HEADER "${PROJECT_SOURCE_DIR}/tests/test_macros.hpp")

macro(add_um2_test FILENAME)
  # Strip the path and extension from the filename to get the test name
  set(TESTNAME ${FILENAME})
  get_filename_component(TESTNAME ${TESTNAME} NAME_WE)
  get_filename_component(TESTNAME ${TESTNAME} NAME_WLE)
  # Prepend "test_" to the test name
  set(TESTNAME "test_${TESTNAME}")

  # Always include the test framework header with the test
  add_executable(${TESTNAME} ${FILENAME} ${TEST_FRAMEWORK_HEADER})

  target_link_libraries(${TESTNAME} PRIVATE um2)

  add_test(${TESTNAME} ${TESTNAME})

  set_target_properties(${TESTNAME} PROPERTIES CXX_STANDARD ${UM2_CXX_STANDARD})

  # clang-tidy
  if (UM2_USE_CLANG_TIDY)
    set_clang_tidy_properties(${TESTNAME})
  endif()

  # cppcheck
  if (UM2_USE_CPPCHECK)
    set_cppcheck_properties(${TESTNAME})
  endif()

  if (UM2_USE_COVERAGE)
    target_link_libraries(${TESTNAME} PRIVATE gcov)
    target_compile_options(${TESTNAME} PRIVATE --coverage)
  endif ()

  # If compiling with CUDA, compile the cpp files as cuda
  if (UM2_USE_CUDA)
    set_cuda_properties(${TESTNAME} ${FILENAME})
  endif()

  # If visualization is enabled, line the necessary libraries
  #  if (UM2_ENABLE_VIS)
  #    target_link_libraries(${TESTNAME} ${UM2_VIS_LIBRARIES})
  #  endif()
endmacro()
