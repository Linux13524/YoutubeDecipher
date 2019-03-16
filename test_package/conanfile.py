import os

from conans import ConanFile, CMake, tools


class YoutubedecipherTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        if tools.cross_building(self.settings):
            return
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        if tools.cross_building(self.settings):
            return

        os.chdir("bin")
        self.run(".%sYoutubeDecipherTest" % os.sep)
