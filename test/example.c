// RUN: %{c2wit_clang} %s | FileCheck %s
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// CHECK: record {
typedef struct {

  // CHECK: ptr: ????
  char *ptr;

  // CHECK: len: u64
  size_t len;
} demo_string_t;
// CHECK: }

// CHECK: record {
typedef struct {
  // CHECK: name: ????
  demo_string_t name;

  // CHECK: age: u32
  uint32_t age;
} demo_person_t;
// CHECK: }
