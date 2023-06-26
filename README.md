# Soft2D
This repository is the official source for the latest releases, user examples and documentation of soft2d. The user documentation is hosted at [here](https://docs.soft2d.tech/).

## Introduction
Soft2D ships header files and binary files of the shared library. These files are under `soft2d/` directory. Additionally, soft2d relies on [Taichi C-API](https://docs.taichi-lang.org/docs/taichi_core) library. The taichi c-api library locates at `dependencies/taichi_c_api/`, which has a same directory structure with `soft2d/`.

### Binary Compatibility
The ABI compatibility of soft2d binary files are listed as follows:
* Linux: **x86_64** (Tested on Ubuntu 20.04 and Ubuntu 22.04)
* Windows: **x86_64** (Tested on Windows 10 with Visual Studio 2022)
* macOS: **arm64-m1** (Tested on Ventura)
* iOS: **aarch64**
* Android: **arm64-v8a**

## User Examples
 ![A preview of user examples](./images/examples.png)

We provide a collection of user examples demonstrating the capabilities of Soft2D. These examples can run on Linux and Windows with Vulkan installed. All examples are tested on Ubuntu 20.04 and Windows 10 with Visual Studio 2022. Follow the steps below to build and run these examples.

### Cloning the Project

* Use `git-lfs` to clone binary files correctly.
* Use `git submodule` to install the necessary third-party dependencies for the vulkan renderer.

``` bash
# git clone ...
# cd soft2d-release/
git lfs pull
git submodule update --init --recursive
```
### Vulkan Installation
Install Vulkan from [Vulkan Official Website](https://www.vulkan.org/), and make sure vulkan-related environment variables are properly configured.

### Running the Examples

For both Linux and Windows users, the following commands can be used to build and execute examples:

#### Linux

* Clean the build directory: `./build_linux.sh --clean`
* Run the minimal test (No GUI): `./build_linux.sh --test`
* Run a specific example: `./build_linux.sh --example=<example_name>`
    * For instance: `./build_linux.sh --example=basic_shapes`
* Build all examples: `./build_linux.sh`
    * All output executables will be located under the `build/` directory.

#### Windows
* Clean the build directory: `.\build_windows.bat --clean`
* Run the minimal test (No GUI): `.\build_windows.bat --test`
* Run a specific example: `.\build_windows.bat --example=<example_name>`
    * For instance: `.\build_windows.bat --example=basic_shapes`
* Build all examples: `.\build_windows.bat`
    * All output executables will be located under the `build/Release/` directory.

