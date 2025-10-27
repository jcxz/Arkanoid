conan install . --build=missing
cmake -S . -B ./build/Release -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./build/Release/generators/conan_toolchain.cmake
