// Copyright (c) 2018 Linus Klöckner

#ifndef LIB_YOUTUBE_DECIPHER_INCLUDE_YOUTUBE_DECIPHER_H_
#define LIB_YOUTUBE_DECIPHER_INCLUDE_YOUTUBE_DECIPHER_H_

#include <string>
#include <tuple>
#include <vector>

namespace Youtube {
    class Decipher {
     public:
        static Decipher& Instance(const std::string& p_video_html) {
            static Decipher _instance(p_video_html);
            return _instance;
        }

        void DecipherSignature(std::string* p_signature);

     private:
        Decipher() = default;
        explicit Decipher(const std::string& p_video_html) {
            LoadDecipher(p_video_html);
        }

        void LoadDecipher(const std::string& p_video_html);

        std::string LoadDecipherJS(const std::string& p_video_html);
        std::string LoadDecipherFuncName(const std::string& p_decipher_js);
        std::string LoadDecipherFuncName2(const std::string& p_decipher_js);
        std::string LoadDecipherFuncDefinition(const std::string& p_decipher_js, const std::string& p_decipher_func_name);
        std::string LoadSubFuncName(const std::string& p_decipher_func_definition);
        std::string LoadSubFuncDefinition(const std::string& p_decipher_js, const std::string& p_sub_func_name);
        void ExtractSubFuncNames(const std::string& p_sub_func_definition);
        void ExtractDecipher(const std::string& p_decipher_func_definition);

        void SubReverse(std::string* p_a);
        void SubSplice(std::string* p_a, int p_b);
        void SubSwap(std::string* p_a, int p_b);

        std::string m_sub_reverse_name;
        std::string m_sub_splice_name;
        std::string m_sub_swap_name;

        std::vector<std::tuple<std::string, int>> m_decipher;
    };
}   // namespace Youtube

#endif   // LIB_YOUTUBE_DECIPHER_INCLUDE_YOUTUBE_DECIPHER_H_
