# Youtube-List-Downloader
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a904a29e8a57484cb79c6d39d9da7061)](https://www.codacy.com/app/Linux13524/Youtube-Decipher?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Linux13524/Youtube-Decipher&amp;utm_campaign=Badge_Grade)

## Description:
This c++-library is able do decipher signatures of Youtube-Videos.

## Usage:
- Include the library into your cmake build
- call Youtube::Decipher::instance(videoHtml).decipherSignature(signature)