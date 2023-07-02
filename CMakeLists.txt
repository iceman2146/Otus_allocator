cmake_minimum_required(VERSION 3.2)

project(allocator)

add_executable(main main.cpp)
add_library(Utils Utils.cpp)

set_target_properties(main Utils  PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(Utils
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(main PRIVATE
    Utils
)



