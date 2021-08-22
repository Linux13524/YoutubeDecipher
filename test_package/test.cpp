#include "YoutubeDecipher/decipher.h"
#include <cpr/cpr.h>
#include <iostream>

int main() {
    std::cout << "Trying to download https://www.youtube.com/watch?v=s8XIgR5OGJc ... " << std::endl;
    auto html_response = cpr::Get(cpr::Url{"https://www.youtube.com/watch?v=s8XIgR5OGJc"}, cpr::VerifySsl{false});
    std::cout << "Trying to load decipher ... ";
    Youtube::Decipher::Instance(html_response.text);
}