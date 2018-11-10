# Medusa

A http server demo written in c++

## Environment

* c++17 compiler
* cmake 3.12
* vcpkg

## Dependent libraries

* nlohmann-json
* restinio
    * http-parser
    * fmtlib
    * asio

## Tips

* For link static in windows vscode add ` "cmake.configureArgs": ["-DVCPKG_TARGET_TRIPLET=x64-windows-static"]` to settings.