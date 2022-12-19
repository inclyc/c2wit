cmake_minimum_required(VERSION 3.10.2)

project(CToWit)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS true)
set (PROJECT_EXEC CToWit)

add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)


option(LOCAL_PROJECT_USE_ASAN "Build with AddressSanitizer" Off)
if(LOCAL_PROJECT_USE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()


option(LOCAL_PROJECT_USE_TSAN "Build with ThreadSanitizer" Off)
if(LOCAL_PROJECT_USE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()

function(append value)
  foreach(variable ${ARGN})
    set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
  endforeach(variable)
endfunction()

if( PROJECT_USE_LINKER )
  append("-fuse-ld=${PROJECT_USE_LINKER}"
    CMAKE_EXE_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS)
endif()

add_compile_options(-Wall -Wextra -pedantic)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_CMAKE_DIR}")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# We incorporate the CMake features provided by LLVM:
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

find_package(Clang REQUIRED CONFIG)

message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
              ${CLANG_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}"
)

include_directories(SYSTEM "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_llvm_library(${PROJECT_EXEC} MODULE src/CToWit.cpp PLUGIN_TOOL clang)

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include)
