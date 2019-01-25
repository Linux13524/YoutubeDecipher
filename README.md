# Youtube-Decipher
## Package Status

| Bintray | Windows | Linux & macOS |
|:--------:|:---------:|:-----------------:|
|[![Download](https://api.bintray.com/packages/linux13524/conan/youtube_decipher%3Alinux13524/images/download.svg) ](https://bintray.com/linux13524/conan/youtube_decipher%3Alinux13524/_latestVersion)|[![Build status](https://ci.appveyor.com/api/projects/status/9g8p38mi3pa8onsp?svg=true)](https://ci.appveyor.com/project/Linux13524/youtube-decipher)|[![Build Status](https://travis-ci.com/Linux13524/Youtube-Decipher.svg)](https://travis-ci.com/Linux13524/Youtube-Decipher)|

## Description:
This c++-library is able do decipher signatures of Youtube-Videos.

### Basic setup

    $ conan install youtube_decipher/1.0.1@linux13524/stable

### Project setup

If you handle multiple dependencies in your project is better to add a *conanfile.txt*

    [requires]
    youtube_decipher/1.0.1@linux13524/testing

    [options]

    [generators]
    cmake

Complete the installation of requirements for your project running:</small></span>

    conan install .

Project setup installs the library (and all his dependencies) and generates the files *conanbuildinfo.txt* and *conanbuildinfo.cmake*
with all the paths and variables that you need to link with your dependencies.

Follow the Conan getting started: http://docs.conan.io

### Usage

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
