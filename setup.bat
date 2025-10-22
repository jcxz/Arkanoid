conan install . --output-folder=build --build=missing -s build_type=Debug
conan install . --output-folder=build --build=missing -s build_type=MinSizeRel
conan install . --output-folder=build --build=missing -s build_type=Release
conan install . --output-folder=build --build=missing -s build_type=RelWithDebInfo
cmake -G"Visual Studio 17 2022" -A x64 -S . -B .\build -DCMAKE_INSTALL_PREFIX=.\install -DCMAKE_TOOLCHAIN_FILE=.\build\conan_toolchain.cmake