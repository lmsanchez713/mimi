cmake_minimum_required(VERSION 3.10)

# set the project name
project(hars VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(hars harstripper.cxx)

#configure_file(prime_config.h.in prime_config.h)

#target_include_directories(cmike PUBLIC "${PROJECT_BINARY_DIR}")

