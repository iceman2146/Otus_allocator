cmake_minimum_required(VERSION 3.26)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(MAIN_APP_NAME "main")

set(PROJECT_NAME "allocator")

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

configure_file(version.h.in version.h) 

include_directories(include)
add_executable(${MAIN_APP_NAME} src/main.cpp)
##add_executable(test test/main_test.cpp test/test_allocator.cpp test/test_container.cpp)
add_library(Utils src/Utils.cpp)

#target_include_directories(
#    test PRIVATE
#    "${CMAKE_CURRENT_SOURCE_DIR}/include"
#)
#target_link_libraries(
#    test PRIVATE
#    gtest gtest_main
#)

set_target_properties(${MAIN_APP_NAME} Utils PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(Utils
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${MAIN_APP_NAME} PRIVATE
    Utils
)



install(TARGETS ${MAIN_APP_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)
include (CPack)
