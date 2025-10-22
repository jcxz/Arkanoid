from conan import ConanFile

class ArkanoidConan(ConanFile):
    name = "Arkanoid"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("sdl/3.2.20")
        self.requires("spdlog/1.16.0")