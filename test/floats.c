// RUN: %{c2wit_clang_cc1} -fsyntax-only %s | FileCheck %s

// CHECK: record Floats {
struct
__attribute__((annotate("wit-export")))
Floats {
    // CHECK: F32: f32
    float F32;
    // CHECK: F64: f64
    double F64;
};
// CHECK: }
