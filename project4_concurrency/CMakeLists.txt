# Set minimum required CMake version
cmake_minimum_required(VERSION 3.10.2)

# Set the C++ standard we will use
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

#  Set the project name
project(concurrency)

# Add include directory (include dir doesn't actually exist)
include_directories(include)

# Locate project dependencies
find_package(Threads REQUIRED)

# Apply link libraries to all targets
# (must come before add_executable or add_library)
link_libraries(Threads::Threads)

# Add project executables
add_executable(threads src/threads.cpp)
add_executable(lambdas src/lambdas.cpp)
add_executable(promises_futures src/promises_futures.cpp)
add_executable(tasks src/tasks.cpp)

# Apply link libraries to specific targets (not needed)
# target_link_libraries(threads Threads::Threads)


