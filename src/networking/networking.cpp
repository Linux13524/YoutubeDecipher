#include "networking/networking.h"

#include <iostream>
#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

namespace ssl = boost::asio::ssl;

using boost::asio::ip::tcp;

typedef ssl::stream<tcp::socket> ssl_socket;

Networking::Https::Response Networking::Https::Get(std::string host,
                                                   std::string query_path,
                                                   std::string request_type) {
    Response http_response = Response();

    try {
        boost::asio::io_service io_service;

        // Create a context that uses the default paths for
        // finding CA certificates.
        ssl::context context(ssl::context::sslv23);
        context.set_default_verify_paths();

        // Try each endpoint until we successfully establish a connection.
        ssl_socket socket(io_service, context);
        boost::asio::connect(socket.lowest_layer(), tcp::resolver(io_service).resolve({host, "https"}));
        socket.lowest_layer().set_option(tcp::no_delay(true));

        socket.set_verify_mode(boost::asio::ssl::verify_none);
        socket.set_verify_callback(boost::asio::ssl::rfc2818_verification(host));
        socket.handshake(ssl_socket::client);

        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << query_path << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: " << request_type << "\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        std::istream response_stream(&response);

        // Check that response is OK.
        response_stream >> http_response.http_version;
        response_stream >> http_response.status_code;
        response_stream >> http_response.status_message;
        response_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!response_stream ||
            http_response.http_version.substr(0, 5) != "HTTP/") {
            return http_response;
        }

        if (http_response.status_code != 200) {
            // TODO: Handle 302
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header_line;
        while (std::getline(response_stream, header_line) && header_line != "\r")
            http_response.header.append(header_line);

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        std::ostringstream ss;
        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error) != 0u) {
            ss << &response;
        }

        http_response.body = ss.str();

        if (error.category() == boost::asio::error::get_ssl_category() &&
            error.value() == ERR_PACK(ERR_LIB_SSL, 0, SSL_R_SHORT_READ)) {
            // -> not a real error, just a normal TLS shutdown
        } else if (error != boost::asio::error::eof) {
            throw boost::system::system_error(error);
        }

        return http_response;
    } catch (std::exception& e) {
        return http_response;
    }
}
