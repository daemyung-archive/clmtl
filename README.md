<h1 align="center">
    <br>
    <img src="https://upload.wikimedia.org/wikipedia/commons/4/4d/OpenCL_logo.svg" width="512">
    <br>
    CLMTL
    <br>
</h1>

<h4 align="center">
    Experimental implementation of OpenCL over Metal.
</h4>

<p align="center">
    <img src="https://github.com/daemyung/clmtl/actions/workflows/build.yml/badge.svg">
    <img src="https://img.shields.io/badge/OpenCL-%3C%3D3.0-yellow">
</p>

<p align="center">
    <a href="#limitations">Limitations</a> •
    <a href="#build">Build</a>
</p>

## Limitations

* Only one device is supported per one context.
* Not support `char8`, `char16`, `uchar8`, `uchar16`, `short8`, `short16`, `ushort8`, `ushort16`, `int8`, `int16`,
  `uint8`, `uint16`, `long8`, `long16`, `ulong8`, `ulong16`, `float8` and `float16`.

## Dependencies

All dependencies are managed by [Conan](https://conan.io/) so install it.

```shell
pip install conan
```

However [Clspv](https://github.com/google/clspv) can't install using [Conan](https://conan.io/). We have to install it
manually.

```shell
git clone https://github.com/daemyung/clspv.git
cd clspv
conan create clspv
```

## Build

Create a build directory, install dependencies and configure CMake.

```shell
mkdir build
conan install . -if build --build=missing
cmake -S . -B build
```

Build all targets.

```shell
cmake --build build
```
