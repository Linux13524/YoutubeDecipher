# Youtube-Decipher
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a904a29e8a57484cb79c6d39d9da7061)](https://www.codacy.com/app/Linux13524/Youtube-Decipher?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Linux13524/Youtube-Decipher&amp;utm_campaign=Badge_Grade)

## Description:
This c++-library is able do decipher signatures of Youtube-Videos.

## Usage:
- Add the library into your cmake build
- Include the `decipher.h` into your code
- call `Youtube::Decipher::instance(videoHtml).DecipherSignature(signature)`

   `videoHtml`: 
   - The html of the desired video (it also may be any other videos html); used to extract the ytplayer.js
      
      It is not needed to provide the whole html due to simple regexing (see section "How does it work?") 
      
   `signature`:
   - The signature you want to decipher
   
## Dependencies:
Uses **Boost-Asio** for network access and **Boost-Regex** for regexing the html and javascript files.

Uses **Open SSL** to perform https request

## How does it work?
will be added in future
