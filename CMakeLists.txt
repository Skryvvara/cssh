cmake_minimum_required(VERSION 3.0)
project(cssh)

# Locate ncurses package
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the version variable
set(VERSION "v0.0.1-dev")

# Add compile options
add_compile_options(-Wall -std=c++17 -DVERSION="${VERSION}")

# Add your source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(cssh ${SOURCES})

# Link ncurses library
target_link_libraries(cssh ${CURSES_LIBRARIES})
