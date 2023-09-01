# LLVM sandbox

A template for experimenting with LLVM. Feel free to clone and have fun :D.

## Requierements

Build LLVM and set LLVM_ROOT variable to the location of where LLVM is installed.
See [find_pacakge() -  config mode search procedure](https://cmake.org/cmake/help/latest/command/find_package.html#id9).

## Build

Clone the repository and set the working directory

```bash
git clone https://github.com/Devil-Syntax-Tree/LLVM_sandbox
cd LLVM_sandbox
```

build the project

```
cmake -S . -B out/<platform>/<build_type> -G Ninja [other parameters...] 
```

or use and configure the CMake presets

```
cmake --preset=<preset_name>
```

go to the build directory and run the executable!

## Notes

On windows you need to link your project with the build type on which LLVM was built.
Or to match the runtime for LLVM and the sandbox.
