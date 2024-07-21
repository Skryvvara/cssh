<div id="top"></div>

<br />
<div align="center">
  <a href="https://github.com/skryvvara/cssh">
      <img src="./.github/assets/cssh.svg" width=124 height=124 style="border-radius: 100%;" alt="CSSH Logo">
  </a>

  <h3 align="center">CSSH - A TUI for ssh written in C++</h3>
  <h4 align="center">Checkout <a href="https://github.com/skryvvara/gossht">gossht</a> wich is a similar project to cssh written in <a href="https://go.dev">go</a></h4>
</div>

## Description

> ! This is a very early proof of concept

CSSH is a graphical ssh helper written in C++ using [ncurses](https://invisible-island.net/ncurses/).

## Build

You can build cssh using cmake, first verify that cmake and a c++ compiler is installed.

```sh
cmake --version

#cmake version 3.30.0
#
#CMake suite maintained and supported by Kitware (kitware.com/cmake).

clang --version

#Apple clang version 15.0.0 (clang-1500.3.9.4)
#Target: arm64-apple-darwin23.5.0
#Thread model: posix
#InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

now run the steps below to compile and run the code

```sh
mkdir build
cd build

cmake ..
make

./cssh
```

## License

CSSH is licensed under the [MIT License](https://opensource.org/license/mit).
