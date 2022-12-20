// RUN: %{ctowit_clang} %s | FileCheck %s

// CHECK: record {
struct Floats {
    // CHECK: F32: f32
    float F32;
    // CHECK: F64: f64
    double F64;
};
// CHECK: }
