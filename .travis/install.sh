#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    brew update || brew update
    brew outdated pyenv || brew upgrade pyenv
    brew install pyenv-virtualenv
    brew install cmake || true

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi

    pyenv install 2.7.10
    pyenv virtualenv 2.7.10 conan
    pyenv rehash
    pyenv activate conan
fi

pip install conan --upgrade
pip install conan_package_tools
pip install https://github.com/lief-project/packages/raw/lief-master-latest/pylief-0.9.0.dev.zip
pip install spdx_lookup

conan user

conan config install http://github.com/conan-io/hooks.git -sf hooks -tf hooks
conan config set hooks.attribute_checker
conan config set hooks.binary_linter
conan config set hooks.bintray_updater
conan config set hooks.conan-center_reviewer
conan config set hooks.github_updater
conan config set hooks.spdx_checker
