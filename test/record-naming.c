// RUN: %{c2wit_clang_cc1} %s | FileCheck %s


struct
__attribute__((annotate("wit-define", "string")))
foo {
    int length;
    char *data;
};


typedef struct foo sugared_foo;


// CHECK: record bar {
struct
__attribute__((annotate("wit-export")))
bar {
    // CHECK-NEXT: a: i32
    int a;

    // CHECK-NEXT: b: string
    struct foo b;

    // // CHECK-NEXT: c: string
    sugared_foo c;
};



