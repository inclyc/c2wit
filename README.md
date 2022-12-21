<div align="center">
  <h1>CToWit</code></h1>

  <p>
    <strong>A language bindings generator for <code>C</code></strong>
  </p>
</div>

## About

This is a clang plugin that generate C records to [.wit](https://github.com/bytecodealliance/wit-bindgen). For existing C projects, you can use this project to generate struct declaration to corresponding wit record.



## Installation

Depend on llvm-project >= 14. 

```
cmake -B build
```

```
cmake --build build
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

Invoke your system clang and load plugin

```
clang -fsyntax-only -fplugin=build/CToWit.so -Xclang -plugin -Xclang ctowit test.c
```

And this plugin convert "Floats" to a .wit record.

```
record {
F32: f32,
F64: f64,
}
```

## Testing

This project cherry-picks the LLVM test suite here, requiring FileCheck and lit. FileCheck should be installed via your package manager, for example `llvm-*-tools` on Debian-based systems. Install `lit` via pip.

```
pip install lit
```

Our regression tests could be run via `lit test`.
