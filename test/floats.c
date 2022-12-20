// // RUN: clang -fsyntax-only -fplugin=build/CToWit.so -Xclang -plugin -Xclang \
// ctowit test/floats.c | FileCheck %s

// CHECK: record {
struct Floats {
    // CHECK: F32: f32
    float F32;
    // CHECK: F64: f64
    double F64;
};
// CHECK: }
