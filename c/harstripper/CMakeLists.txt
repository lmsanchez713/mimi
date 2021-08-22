cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    # set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# set the project name
project(hars VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nlohmann_json REQUIRED)

# add the executable
add_executable(hars harstripper.cxx)
target_link_libraries(hars nlohmann_json::nlohmann_json)

#configure_file(prime_config.h.in prime_config.h)

#target_include_directories(cmike PUBLIC "${PROJECT_BINARY_DIR}")

