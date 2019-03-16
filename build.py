from cpt.packager import ConanMultiPackager
import platform
import os


if __name__ == "__main__":
    is_android = "android" in os.getenv("CONAN_BASE_PROFILE", "")
    is_windows = platform.system() == "Windows"

    if is_android:
        os.system("conan config install %s" % os.getenv("CONAN_CONFIG_URL"))

    builder = ConanMultiPackager()
    builder.add_common_builds(shared_option_name=False, pure_c=False)

    filtered_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:

        # MinGW should use "libstdc++11"
        if is_windows and settings["compiler"] == "gcc":
            settings["compiler.libcxx"] = "libstdc++11"

        if is_android:
            if settings["compiler.libcxx"] != "libc++":
                continue

        filtered_builds.append([settings, options, env_vars, build_requires])
    builder.builds = filtered_builds

    builder.run()
