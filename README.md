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

## Build

Create a build directory and configure CMake.

```shell
mkdir build
cmake -S . -B build
```

Build all targets.

```shell
cmake --build build
```
