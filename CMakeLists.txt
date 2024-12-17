#Actually required minimum CMake version is not 3.10.0, it's only uses basic features
cmake_minimum_required(VERSION 3.10.0)
project(MatrixDeterminantCalculator VERSION 0.1.0 LANGUAGES C)

add_executable(MatrixDeterminantCalculator main.c)

#[[ It's add support for testing and packing
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
]]
