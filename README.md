# soft2d-release
Soft2D is a 2D multi-material continuum physics engine designed for real-time applications.

This repo hosts the newest soft2d releases and the user examples.

### Links

### Release (Binaries) Availability

## Example Availability
Support Linux and Windows. Vulkan is required. Tested on Ubuntu 20.04 and Windows 10, Visual Studio 2022.

 ![A preview of user examples](./images/examples.png)

## Set-up

### Clone repo
``` bash
git clone ...
git lfs pull
git submodule update --init --recursive
```
### Install Vulkan

## Building and Running

### Linux

* Clean build directory: `./build_linux.sh --clean`
* Minimal Test: `./build_linux.sh --test`
* Build and run a specific example: `./build_linux.sh --example=<example_name>`
    * For example: `./build_linux.sh --example=basic_shapes`
* Build all examples: `./build_linux.sh`
    * All output executables will be stored under the `build/` directory.

### Windows
* Clean build directory: `.\build_windows.bat --clean`
* Minimal Test: `.\build_windows.bat --test`
* Build and run a specific example: `.\build_windows.bat --example=<example_name>`
    * For example: `.\build_windows.bat --example=basic_shapes`
* Build all examples: `.\build_windows.bat`
    * All output executables will be stored under the `build/Release/` directory.

