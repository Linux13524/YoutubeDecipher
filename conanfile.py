from conans import ConanFile, CMake
import os


class YoutubeDecipherConan(ConanFile):
    name = "youtube_decipher"
    version = "1.0.1"
    license = "MIT"
    author = "Linus Kloeckner (linus.kloeckner@googlemail.com)"
    url = "https://github.com/Linux13524/Youtube-Decipher"
    description = "Library to decipher youtube signatures"
    topics = ("youtube", "decipher", "signature")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = "shared=False", "fPIC=True"
    requires = "boost/1.66.0@conan/stable", "cpr/1.3.0@linux13524/stable"
    generators = "cmake"
    exports_sources = "package/*"

    def build(self):
        cmake = CMake(self)
        os.mkdir("build")
        cmake.configure(source_folder="package")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="package/include")
        self.copy("*.a", dst="lib", src="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", src="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)

    def package_info(self):
        LIB_POSTFIX = "-d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["YoutubeDecipher" + LIB_POSTFIX]
