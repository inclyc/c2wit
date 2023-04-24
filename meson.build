project(
    'CToWit',
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

library('CToWit', ['src/CToWit.cpp'], dependencies: [clang_dep, dependency('llvm')], install: true)
