[![Download](https://api.bintray.com/packages/linux13524/conan/youtube_decipher%3Alinux13524/images/download.svg) ](https://bintray.com/linux13524/conan/youtube_decipher%3Alinux13524/_latestVersion)
[![Build Status Travis](https://travis-ci.com/Linux13524/YoutubeDecipher.svg?branch=testing%2F1.0.1)](https://travis-ci.com/Linux13524/YoutubeDecipher)
[![Build Status AppVeyor](https://ci.appveyor.com/api/projects/status/9g8p38mi3pa8onsp/branch/testing/1.0.1?svg=true)](https://ci.appveyor.com/project/linux13524/youtube-decipher)

## Conan package recipe for *youtube_decipher*

Library to decipher youtube signatures

The packages generated with this **conanfile** can be found on [Bintray](https://bintray.com/linux13524/conan/youtube_decipher%3Alinux13524).

## For Users

### Basic setup

    $ conan install youtube_decipher/1.0.1@linux13524/testing

### Project setup

If you handle multiple dependencies in your project is better to add a *conanfile.txt*

    [requires]
    youtube_decipher/1.0.1@linux13524/testing

    [generators]
    cmake

Complete the installation of requirements for your project running:

    $ mkdir build && cd build && conan install ..

Note: It is recommended that you run conan install from a build directory and not the root of the project directory.  This is because conan generates *conanbuildinfo* files specific to a single build configuration which by default comes from an autodetected default profile located in ~/.conan/profiles/default .  If you pass different build configuration options to conan install, it will generate different *conanbuildinfo* files.  Thus, they should not be added to the root of the project, nor committed to git.


## Build and package

The following command both runs all the steps of the conan file, and publishes the package to the local system cache.  This includes downloading dependencies from "build_requires" and "requires" , and then running the build() method.

    $ conan create . linux13524/testing


### Available Options
| Option        | Default | Possible Values  |
| ------------- |:----------------- |:------------:|
| shared      | False |  [True, False] |
| fPIC      | True |  [True, False] |


## Add Remote

    $ conan remote add linux13524 "https://api.bintray.com/conan/linux13524/conan"

## Usage

- Include the `decipher.h` into your code
- call `Youtube::Decipher::instance(videoHtml).DecipherSignature(signature)`

   `videoHtml`: 
   - Any Youtube video HTML (used to extract the ytplayer.js)
      
      It is not necessary to provide the whole html due to simple regexing (see section "How does it work?") 
      
   `signature`:
   - The signature you want to decipher
   
## Dependencies:
`boost/1.66.0@conan/stable`, `cpr/1.3.0@linux13524/stable`

## How does it work?
See the [wiki](https://github.com/Linux13524/Youtube-Decipher/wiki)

## License

[MIT](https://github.com/Linux13524/Youtube-Decipher/blob/testing/1.0.1/LICENSE.md)
