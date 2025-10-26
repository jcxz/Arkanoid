import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy


class ArkanoidRecipe(ConanFile):
    name = "Arkanoid"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("sdl/3.2.20")
        self.requires("spdlog/1.16.0")
        self.requires("imgui/1.92.4")
        self.requires("glew/2.2.0")
        self.requires("glm/1.0.1")

    def generate(self):
        bindings_src = os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings")
        bindings_dst = os.path.join(self.build_folder, "external")
        copy(self, "*sdl3*",    bindings_src, bindings_dst)
        copy(self, "*opengl3*", bindings_src, bindings_dst)

    def layout(self):
        cmake_layout(self)