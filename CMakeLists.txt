cmake_minimum_required(VERSION 3.5)

project (posix-utils)

option(BUILD_TESTS "Build test programs" ON)


set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic )

#Don't really like this include
include_directories(${PROJECT_SOURCE_DIR}/shared-buffer/include)
include_directories(${PROJECT_SOURCE_DIR}/synchronization/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(shared-buffer/)
add_subdirectory(synchronization/)


#Add unit tests
if( BUILD_TESTS )
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(tests/)
endif()


