# Set minimum required CMake version
cmake_minimum_required(VERSION 3.10.2)

# Set the C++ standard we will use
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

#  Set the project name
project(concurrency)

# Add include directory
include_directories(include)

# Locate project dependencies
find_package(Threads REQUIRED)

# Apply link libraries to all targets
# (must come before add_executable or add_library)
link_libraries(Threads::Threads)

# Add project executables
add_executable(threads src/threads.cpp)

# Apply link libraries to specific targets
# target_link_libraries(threads Threads::Threads)


