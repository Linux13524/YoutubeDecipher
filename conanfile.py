from conans import ConanFile, CMake


class YoutubeDecipherConan(ConanFile):
    name = "youtube_decipher"
    version = "1.0.1"
    license = "MIT"
    author = "Linus Kloeckner (linus.kloeckner@googlemail.com)"
    url = "https://github.com/Linux13524/YoutubeDecipher"
    description = "Library to decipher youtube signatures"
    homepage = "https://www.linux13524.de"
    topics = ("youtube", "decipher", "signature")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = "shared=False", "fPIC=True"
    generators = "cmake"
    exports = ["LICENSE.md"]
    exports_sources = "src/*"

    def requirements(self):
        self.requires("boost/1.69.0")
        self.requires("cpr/1.3.0")
        self.requires("nlohmann_json/3.10.0")

    def configure(self):
        if self.settings.compiler == 'Visual Studio':
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.install()
        # Copy sources for debugging
        if self.settings.build_type == "Debug":
            self.copy("*.cpp", dst="src", src="package/src")
        # Copy the license files
        self.copy("LICENSE.md", dst="licenses")

    def package_info(self):
        LIB_POSTFIX = "-d" if self.settings.build_type == "Debug" else ""
        self.cpp_info.libs = ["YoutubeDecipher" + LIB_POSTFIX]
        self.cpp_info.srcdirs = "src"
