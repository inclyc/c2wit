// RUN: %{ctowit_clang} %s | FileCheck %s
#include <stdint.h>

// CHECK: record Test {
struct Test {
  // CHECK: c: i8
  char c;
  // CHECK: s: i16
  short s;
  // CHECK: i: i32
  int i;
  // CHECK: l: i64
  long l;
  // CHECK: ll: i64
  long long ll;

  // CHECK: uc: u8
  unsigned char uc;
  // CHECK: us: u16
  unsigned short us;
  // CHECK: ui: u32
  unsigned int ui;
  // CHECK: ul: u64
  unsigned long ul;
  // CHECK: ull: u64
  unsigned long long ull;

  // CHECK: u32: u32
  uint32_t u32;
  // CHECK: u64: u64
  uint64_t u64;

  // CHECK: i32: i32
  int32_t i32;

  // CHECK: i64: i64
  int64_t i64;
};
// CHECK: }
