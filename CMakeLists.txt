cmake_minimum_required(VERSION 3.12)
set(VCPKG_CRT_LINKAGE static)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
endif()

project(Medusa CXX)

set(CMAKE_CXX_STANDARD 17)


find_package(unofficial-http-parser REQUIRED)
find_package(fmt REQUIRED)
find_package(restinio REQUIRED)
find_path(NLOHMANNJSON_INCLUDE_DIR NAMES json.hpp PATH_SUFFIXES nlohmann)
add_executable(medusa src/main.cpp)
target_include_directories(medusa PRIVATE ${NLOHMANNJSON_INCLUDE_DIR})
target_link_libraries(medusa PRIVATE restinio::restinio)