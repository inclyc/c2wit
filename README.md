<div align="center">
  <h1>C2Wit</code></h1>

  <p>
    <strong>A language bindings generator for <code>C</code></strong>
  </p>
</div>

## About

This is a tool that generate C declarations to [.wit](https://github.com/bytecodealliance/wit-bindgen).
For existing C projects, you can use this project to generate declarations to corresponding wit file.

Note that we provide a standalone clang-tool `c2wit` and a clang plugin `libC2WitPlugin.so`.
They share the same code about wit-codegen, but expose different user interfaces, for your choice.


## Installation

Depend on llvm-project >= 14. You may need `clang-cpp.so` provided in LLVM libs directory, should be provided by your package manager.

```
meson setup build --buildtype=release
```

```
meson compile -C build
```


## Usage

Declaration having attribute `__attribute__((annotate("wit-export")))` will be exported.

An example translation unit:

```C
/* test.c */
struct
__attribute__((annotate("wit-export")))
Floats {
    float F32;
    double F64;
};
```

<details>
<summary><b>(a) Use a clang plugin</b></summary>

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

</details>
<details>
<summary><b>(b) Use the standalone tool</b></summary>

Here we provide a standalone executable that could be invoked directly.

```
c2wit test.c
```

Based on [libTooling](https://clang.llvm.org/docs/LibTooling.html),
you may specify a compilation database, used to find header files & definitions the translation unit.

</details>


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

struct
__attribute__((annotate("wit-export")))
bar {
    int a;

    struct foo b;

    sugared_foo c;
};
```

```
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
