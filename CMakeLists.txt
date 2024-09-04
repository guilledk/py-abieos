cmake_minimum_required(VERSION 3.14)
project(py_abieos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/libraries)

pybind11_add_module(
    _abieos
    MODULE
    ${CMAKE_SOURCE_DIR}/src/_abieos.cpp
    ${CMAKE_SOURCE_DIR}/libraries/abieos/src/abieos.cpp
)

target_link_libraries(_abieos PUBLIC abieos -static-libgcc -static-libstdc++)
target_include_directories(_abieos PRIVATE ${CMAKE_SOURCE_DIR}/libraries/abieos/src)
