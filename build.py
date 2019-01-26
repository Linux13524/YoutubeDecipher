from cpt.packager import ConanMultiPackager
import platform


if __name__ == "__main__":
    builder = ConanMultiPackager()
    builder.add_common_builds(shared_option_name=False, pure_c=False)

    if platform.system() == "Windows":
        filtered_builds = []
        for settings, options, env_vars, build_requires in builder.builds:
            # MinGW should use "libstdc++11"
            if settings["compiler"] == "gcc":
                settings["compiler.libcxx"] = "libstdc++11"
            filtered_builds.append([settings, options, env_vars, build_requires])
        builder.builds = filtered_builds

    builder.run()
