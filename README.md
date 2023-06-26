# soft2d-release

This repo holds the newest soft2d releases and the user examples [demos]().
### Links

### Release (Binaries) Availability

## Example Availability
Support Linux and Windows. Vulkan is required. Tested on Ubuntu 20.04 and Windows 10, Visual Studio 2022.

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

