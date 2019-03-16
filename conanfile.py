from conans import ConanFile, CMake
import os


class YoutubeDecipherConan(ConanFile):
    name = "youtube_decipher"
    version = "1.0.1"
    license = "MIT"
    author = "Linus Kloeckner (linus.kloeckner@googlemail.com)"
    url = "https://github.com/Linux13524/YoutubeDecipher"
    description = "Library to decipher youtube signatures"
    topics = ("youtube", "decipher", "signature")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = "shared=False", "fPIC=True"
    requires = "boost/1.69.0@conan/stable", "cpr/1.3.0@linux13524/stable"
    generators = "cmake"
    exports_sources = "package/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="package")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.configure(source_folder="package")
        cmake.install()
        # Copy sources for debugging
        if self.settings.build_type == "Debug":
            self.copy("*.cpp", dst="src", src="package/src")

    def package_info(self):
        LIB_POSTFIX = "-d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["YoutubeDecipher" + LIB_POSTFIX]
        self.cpp_info.srcdirs = "src"
