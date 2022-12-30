project(
    'CToWit',
    'cpp',
    default_options: [
        'warning_level=3',
        'werror=false',
        'cpp_std=gnu++20',
        'cpp_eh=none'
    ]
)

llvm_dep = dependency('llvm')

add_global_arguments('-Wno-unused-parameter', language: 'cpp')

cpp = meson.get_compiler('cpp')

clang_include = llvm_dep.get_configtool_variable('includedir')
clang_libdir = llvm_dep.get_configtool_variable('libdir').split(' ')

clang_dep = declare_dependency(
    dependencies: cpp.find_library('clang-cpp', dirs: clang_libdir),
    include_directories: include_directories(clang_include),
)

dep_libs = [clang_dep, llvm_dep]

library('CToWit', ['src/CToWit.cpp'], dependencies: dep_libs)
