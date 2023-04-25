// RUN: %{c2wit_clang_cc1} %s | FileCheck %s


// CHECK: foo: func(a: i32, b: f32, c: f64) -> i32
__attribute__((annotate("wit-export"))) int foo(int a, float b, double c);

__attribute__((annotate("wit-export")))
__attribute__((annotate("wit-name", "bar")))
// CHECK: bar: func(a: i32, b: f32, c: f64) -> i32
int foz(int a, float b, double c);


// TODO: naming params using attribute
__attribute__((annotate("wit-export")))
// CHECK: foz2: func(: i32, : f32, c: f64) -> i32
int foz2(int, float, double c);


__attribute__((annotate("wit-name", "bar")))
// CHECK-EMPTY:
int baz(int a, float b, double c);

