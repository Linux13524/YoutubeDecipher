#ifndef YOUTUBE_DECIPHER_NETWORKING_H
#define YOUTUBE_DECIPHER_NETWORKING_H

#include <string>

namespace Networking {

    class Https {
    public:
        struct Response {
            std::string http_version;
            unsigned int status_code;
            std::string status_message;
            std::string header;
            std::string body;
        };

        static Response Get(std::string host, std::string query_path,
                            std::string request_type);
    };

};  // namespace Networking

#endif  // YOUTUBE_DECIPHER_NETWORKING_H
