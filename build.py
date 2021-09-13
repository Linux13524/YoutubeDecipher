from cpt.packager import ConanMultiPackager
import platform
import os


def docker_entry_script():
    return " ".join(["conan config install http://github.com/conan-io/hooks.git -sf hooks -tf hooks &&",
                     "conan config set hooks.attribute_checker &&",
                     "conan config set hooks.github_updater &&",
                     "conan config set hooks.spdx_checker"])


def pip_extra_packages():
    return ["spdx_lookup"]


if __name__ == "__main__":
    is_android = "android" in os.getenv("CONAN_BASE_PROFILE", "")
    is_windows = platform.system() == "Windows"

    if is_android:
        os.system("conan config install %s" % os.getenv("CONAN_CONFIG_URL"))

    builder = ConanMultiPackager(docker_entry_script=docker_entry_script(), pip_install=pip_extra_packages())
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
