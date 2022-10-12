# Building VirtualRover
Do this only if necessary! A windows executable is already built in this project. 

## Build VirtualRover on LINUX

### Install/configure vcpkg
from this repository's root directory:
```
    bash utils/Install_vcpkg.sh
```

*This will install and initialize VCPKG, a third-party tool used to build libraries*
### If you are using CLion:
- In `File/Settings/"Build, execution, deployment" ` change your configuration (or create a new one): 
  - Name: *Insert a name*
  - Build type: *debug* or *release*
  - Generator *Let CMake Decide*
  - Cmake Option *-DCMAKE_TOOLCHAIN_FILE=<full-path-to-this-repository's-root-directory>\vcpkg-windows\scripts\buildsystems\vcpkg.cmake*

### Directly using CMake:
    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=<<full-path-to-this-repository's-root-directory>\vcpkg\scripts\buildsystems\vcpkg.cmake ..
    make

## Run VirtualRover:
### If you are using CLion:
from now you should be able to build and run VirtualRover from CLion

### Direct run:
    cd build/src/VirtualRover
    ./VirtualRover

## Build VirtualRover on WINDOWS

### Install/configure vcpkg
```
from this repository's root directory:
    utils\Install_vcpkg.bat
```

*This will install and initialize VCPKG, a third-party tool used to build libraries*

### If you are using CLion:
- In `File/Settings/"Build, execution, deployment"` change your configuration (or create a new one):
    - Name: *Insert a name*
    - Build type: *debug* or *release*
    - Generator *Let CMake Decide*
    - Cmake Option -DCMAKE_TOOLCHAIN_FILE=<path-to-this-repository's-root-directory>/vcpkg/scripts/buildsystems/vcpkg.cmake

### Directly using CMake:
    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=<path-to-this-repository's-root-directory>/vcpkg/scripts/buildsystems/vcpkg.cmake ..
    make

## Run VirtualRover:
### If you are using CLion:
from now, you should be able to build and run VirtualRover from CLion

### Direct run:
    cd build/src/VirtualRover
    ./VirtualRover
