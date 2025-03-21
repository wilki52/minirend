cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
file(GLOB SOURCES "src/*.cpp")
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# This assumes the SDL source is avaiusual
#add_executable(hello hello.cpp)
add_executable(minirend ${SOURCES})
target_include_directories(minirend PUBLIC include)

# Link to the actual SDL3 library.
target_link_libraries(minirend PRIVATE SDL2::SDL2)
