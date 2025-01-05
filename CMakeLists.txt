cmake_minimum_required(VERSION 3.10)

project(Sild)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# SDL2 paths
set(SDL2_INCLUDE_DIR "/opt/homebrew/opt/sdl2/include")
set(SDL2_LIBRARY "/opt/homebrew/opt/sdl2/lib/libSDL2.dylib")

# Add executable and source files
add_executable(program src/main.cpp)

# Link SDL2 explicitly
target_include_directories(program PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(program PRIVATE ${SDL2_LIBRARY})
