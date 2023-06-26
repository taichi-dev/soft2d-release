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
```
### Install Vulkan

## Building and Running

### Linux

#### Clean Build Directory
``` bash
./build_linux --clean
```

#### Minimal Test
``` bash
./build_linux --test
```

#### Examples
``` bash
./build_linux -e=<example_name> 
# e.g. ./build_linux -e=basic_shapes
```
##### Building All Examples
``` bash
./build_linux
```
All output executables will be stored under the build/ directory.


### Windows

