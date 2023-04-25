<div align="center">
  <h1>C2Wit</code></h1>

  <p>
    <strong>A language bindings generator for <code>C</code></strong>
  </p>
</div>

## About

This is a tool that generate C records to [.wit](https://github.com/bytecodealliance/wit-bindgen).
For existing C projects, you can use this project to generate struct declaration to corresponding wit record.



## Installation

Depend on llvm-project >= 14. You may need `clang-cpp.so` provided in LLVM libs directory, should be provided by your package manager.

```
meson setup build --buildtype=release
```

```
meson compile -C build
```


## Usage

An example translation unit:

```C
/* test.c */
struct Floats {
    float F32;
    double F64;
};
```

Invoke your system clang and load the plugin

```
clang -fsyntax-only -fplugin=build/C2Wit.so -Xclang -plugin -Xclang c2wit test.c
```

And this plugin converts "Floats" to a .wit record.

```
record Floats {
F32: f32,
F64: f64,
}
```

### Record naming

Mark the record as `__attribute__(("wit-define", <name>))`, gives this record a name, types desugared (i.e. no typedefs, typeofs) are considered the same.
This is useful to mark a struct a "string", because we do not have native strings in C/C++.


```c++
struct
__attribute__((annotate("wit-define", "string")))
foo {
    int length;
    char *data;
};


typedef struct foo sugared_foo;

struct bar {
    int a;

    struct foo b;

    sugared_foo c;
};
```

```
record foo {
length: i32,
data: ????,
}
record bar {
a: i32,
b: string,
c: string,
}
```

## Testing

This project cherry-picks the LLVM test suite here, requiring `FileCheck` and `lit`. `FileCheck` should be installed via your package manager, for example `llvm-*-tools` on Debian-based systems. Install `lit` via pip.

```
pip install lit
```

Our regression tests could be run via `lit test`.
