cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("ZiYue4DPass")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(ZiYue4DPass SHARED RemoveUnusedStringPass.cpp ZiYue4DPass.def
        ZiYue4DPass.cpp
)

llvm_map_components_to_libnames(llvm_libs core support passes)

target_include_directories(ZiYue4DPass PRIVATE ${LLVM_INCLUDE_DIRS})