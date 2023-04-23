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

clang_dep = cpp.find_library('clang-cpp', has_headers: ['clang/AST/ASTConsumer.h'])

dep_libs = [clang_dep, llvm_dep]

library('CToWit', ['src/CToWit.cpp'], dependencies: dep_libs)
