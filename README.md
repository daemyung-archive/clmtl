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
    <img src="https://img.shields.io/badge/1.0-in%20progress-yellow">
</p>

<p align="center">
    <a href="#limitations">Limitations</a> •
    <a href="#build">Build</a>
</p>

## Limitations

* Only one device is supported per one context.

## Build

Create a build directory and generate build script. 

```shell
mkdir build
cmake -S . -B build
```

Build all targets.

```shell
cmake --build build
```
