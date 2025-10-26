import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy


class Arkanoid(ConanFile):
    name = "Arkanoid"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("sdl/3.2.20")
        self.requires("spdlog/1.16.0")
        self.requires("imgui/1.92.4")
        self.requires("glew/2.2.0")

    def generate(self):
        copy(self, "*sdl3*",         os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"), os.path.join(self.source_folder, "external"))
        copy(self, "*sdlrenderer3*", os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"), os.path.join(self.source_folder, "external"))

    def layout(self):
        cmake_layout(self)