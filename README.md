A simple Arkanoid clone. Works on Windows, macOS and Linux (tested only compilation via wsl on Windows)

#### Prerequisites

- conan
- cmake
- Visual Studio (on Windows)

#### Building Windows

If you have installed all the prerequisites listed above just double click on `setup.bat` and open the generated Visual Studio solution from the build folder (if you want to use something other than Visual Studio you will have to modify the `setup.bat` script).

#### Building Linux/macOS

If you have conan and cmake installed, then just run the following commands from terminal:

```sh
./setup.sh
cmake --build ./build/Release
./build/Release/Arkanoid
```