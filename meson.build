project(
    'C2Wit',
    'cpp', 'c',
    default_options: [
        'warning_level=3',
        'werror=false',
        'cpp_std=gnu++20',
        'cpp_eh=none'
    ]
)

add_global_arguments('-Wno-unused-parameter', language: 'cpp')

# https://github.com/mesonbuild/meson/issues/10097#issuecomment-1063228735
clang_dep = dependency('Clang', method : 'cmake', modules: [
  'clangBasic', 'clangAST', 'clangCodeGen', 'clangLex',
  'clangDriver', 'clangFrontend', 'clangFrontendTool',
  'clangHandleCXX', 'clangHandleLLVM', 'clangSerialization',
  'clangSema', 'clangParse', 'clangEdit', 'clangAnalysis'
])

llvm_dep = dependency('llvm')
fmt_dep = dependency('fmt')

deps = [clang_dep, llvm_dep, fmt_dep]

incdir = include_directories('include')

c2wit_lib = library('C2Wit', ['lib/Consumer.cpp', 'lib/Support.cpp'], dependencies: deps, install: true, include_directories : incdir)

library('C2WitPlugin', ['tools/Plugin.cpp'], dependencies: deps, install: true, link_with: c2wit_lib, include_directories : incdir)
