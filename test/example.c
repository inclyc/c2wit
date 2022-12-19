#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *ptr;
  size_t len;
} demo_string_t;

typedef struct {
  demo_string_t name;
  uint32_t age;
} demo_person_t;
