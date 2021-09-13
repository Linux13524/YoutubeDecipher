// Copyright (c) 2018 Linus Klöckner

#include "YoutubeDecipher/decipher.h"

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <regex>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

void Youtube::Decipher::DecipherSignature(std::string* p_signature) {
    for (const auto& sub : m_decipher) {
        const std::string& p_func_name = std::get<0>(sub);
        if (p_func_name == m_sub_reverse_name) {
            SubReverse(p_signature);
        } else if (p_func_name == m_sub_splice_name) {
            SubSplice(p_signature, std::get<1>(sub));
        } else if (p_func_name == m_sub_swap_name) {
            SubSwap(p_signature, std::get<1>(sub));
        }
    }
}

void Youtube::Decipher::LoadDecipher(const std::string& p_video_html) {
    std::string decipher_js = LoadDecipherJS(p_video_html);
    std::string decipher_func_name = LoadDecipherFuncName(decipher_js);
    std::string decipher_func_definition = LoadDecipherFuncDefinition(decipher_js, decipher_func_name);
    std::string sub_func_name = LoadSubFuncName(decipher_func_definition);
    std::string sub_func_definition = LoadSubFuncDefinition(decipher_js, sub_func_name);
    ExtractSubFuncNames(sub_func_definition);
    ExtractDecipher(decipher_func_definition);
}

std::string Youtube::Decipher::LoadDecipherJS(const std::string& p_video_html) {
    // RegExes
    std::regex expr_player_url(R"((?:PLAYER_JS_URL|jsUrl)\"\s*:\s*\"([^"]+))");

    // Matches
    std::smatch matches_player_url;

    // Search player URL
    std::regex_search(p_video_html, matches_player_url, expr_player_url);

    if (matches_player_url.empty()) {
        throw std::runtime_error("Could not find player URL!");
    }

    std::string str_decipher = matches_player_url[1];

    // Load video player JS
    cpr::Response html_response = cpr::Get(cpr::Url{"https://www.youtube.com" + str_decipher},
                                           cpr::VerifySsl{false}); // cpr currently not supports ca files

    return html_response.text;
}

std::string Youtube::Decipher::LoadDecipherFuncName(const std::string& p_decipher_js) {
    // RegExes
    std::vector<std::regex> regex_list = {
        std::regex(R"(\b[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()"),
        std::regex(R"(\b[a-zA-Z0-9]+\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()"),
        std::regex(R"(\bm=([a-zA-Z0-9$]{2})\(decodeURIComponent\(h\.s\)\))"),
        std::regex(R"(\bc&&\(c=([a-zA-Z0-9$]{2})\(decodeURIComponent\(c\)\))"),
        std::regex(R"((?:\b|[^a-zA-Z0-9$])([a-zA-Z0-9$]{2})\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\);[a-zA-Z0-9$]{2}\.[a-zA-Z0-9$]{2}\(a,\d+\))"),
        std::regex(R"((?:\b|[^a-zA-Z0-9$])([a-zA-Z0-9$]{2})\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\))"),
        std::regex(R"(([a-zA-Z0-9$]+)\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\))")
    };
    
    // Matches
    std::smatch matches_decipher_func;

    // Search decipher function
    for (auto& regex_expr : regex_list) {
        std::regex_search(p_decipher_js, matches_decipher_func, regex_expr);
        if (!matches_decipher_func.empty()) {
            return matches_decipher_func[1];
        }
    }
    throw std::runtime_error("Could not find decipher function name!");
}

std::string Youtube::Decipher::LoadDecipherFuncDefinition(const std::string& p_decipher_js, const std::string& p_decipher_func_name) {
    // RegExes
    std::regex expr_sub_func_definition(p_decipher_func_name + R"(=function\(.+?\)\{(.+?)\})");

    // Matches
    std::smatch matches_sub_funcs;

    // Search decipher function definition
    std::regex_search(p_decipher_js, matches_sub_funcs, expr_sub_func_definition);
    return matches_sub_funcs[1];
}

std::string Youtube::Decipher::LoadSubFuncName(const std::string& p_decipher_func_definition) {
    // RegExes
    std::regex expr_sub_func_name(R"((..)\.(..)\(.,(\d)+\))");

    // Matches
    std::smatch matches_sub_func;

    std::stringstream ss(p_decipher_func_definition);
    std::string item;
    while (std::getline(ss, item, ';')) {
        std::regex_search(item, matches_sub_func, expr_sub_func_name);
        std::string str_sub_func_name = matches_sub_func[1];

        if (!str_sub_func_name.empty())
            return str_sub_func_name;
    }
}

std::string Youtube::Decipher::LoadSubFuncDefinition(const std::string& p_decipher_js,
    const std::string& p_sub_func_name) {
    // SubFuncName may contain special chars like $
    std::string fixed_sub_func_name(p_sub_func_name);
    std::string special_chars = "@&+-";   // TODO: add possible chars used by youtube

    // Fix the sub-function name to be detected correctly by regex in case it contains a special character
    for (auto c : special_chars) {
        auto pos = fixed_sub_func_name.find(c);
        if (pos != std::string::npos) {
            std::string fixed_char("\\");
            fixed_char.append(1, c);
            fixed_sub_func_name.replace(pos, 1, fixed_char);
        }
    }
    
    // RegExes
    std::regex expr_sub_func_definition(R"(var\s)" + fixed_sub_func_name + R"(=\{((?:\n|.)*?)\};)");

    // Matches
    std::smatch matches_sub_func_definitions;

    std::regex_search(p_decipher_js, matches_sub_func_definitions, expr_sub_func_definition);
    return matches_sub_func_definitions[1];
}

void Youtube::Decipher::ExtractSubFuncNames(const std::string& p_sub_func_definition) {
    // RegExes
    std::regex expr_sub_func_names(R"((\w\w):function\(.+?\)\{(.*?)\})");

    // Iterator ends
    std::sregex_iterator iter_end;

    // Qualities
    std::sregex_iterator iter_sub_func_names(p_sub_func_definition.begin(), p_sub_func_definition.end(), expr_sub_func_names);

    while (iter_sub_func_names != iter_end) {
        std::smatch matches_sub_func_name = *iter_sub_func_names++;

        std::string str_def = matches_sub_func_name[2];

        if (str_def.find("reverse") != std::string::npos)
            m_sub_reverse_name = matches_sub_func_name[1];
        else if (str_def.find("splice") != std::string::npos)
            m_sub_splice_name = matches_sub_func_name[1];
        else
            m_sub_swap_name = matches_sub_func_name[1];
    }
}

void Youtube::Decipher::ExtractDecipher(const std::string& p_decipher_func_definition) {
    // Create "decipher"
    // RegExes
    std::regex expr_sub_func(R"(\.(..)\(.,(\d+)\))");

    // Matches
    std::smatch matches_sub_func;

    std::stringstream ss(p_decipher_func_definition);
    std::string item;
    while (std::getline(ss, item, ';')) {
        if (!std::regex_search(item, matches_sub_func, expr_sub_func))
            continue;

        std::string func_name = matches_sub_func[1];
        std::string func_arg_test = matches_sub_func[2];
        int func_arg = stoi(matches_sub_func[2]);

        m_decipher.emplace_back(func_name, func_arg);
    }
}

void Youtube::Decipher::SubReverse(std::string* p_a) {
    std::reverse(p_a->begin(), p_a->end());
}

void Youtube::Decipher::SubSplice(std::string* p_a, int p_b) {
    p_a->erase(0, static_cast<uint64_t>(p_b));
}

void Youtube::Decipher::SubSwap(std::string* p_a, int p_b) {
    char c = (*p_a)[0];
    (*p_a)[0] = (*p_a)[p_b % p_a->length()];
    (*p_a)[p_b] = c;
}
