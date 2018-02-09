#ifndef YOUTUBE_LIST_DOWNLOADER_DECIPHER_H
#define YOUTUBE_LIST_DOWNLOADER_DECIPHER_H

#include <string>
#include <vector>
#include <tuple>

namespace Youtube {
    class Decipher {
    public:
        static Decipher& instance(std::string& videoHtml) {
            static Decipher _instance(videoHtml);
            return _instance;
        }

        void decipherSignature(std::string &signature);

    private:
        Decipher() = default;
        explicit Decipher(std::string& videoHtml) {
            loadDecipher(videoHtml);
        };

        void loadDecipher(std::string& videoHtml);

        std::string loadDecipherJS(std::string& videoHtml);
        std::string loadDecipherFuncName(std::string& decipherJs);
        std::string loadDecipherFuncDefinition(std::string& decipherJs, std::string& decipherFuncName);
        std::string loadSubFuncName(std::string& decipherFuncDefinition);
        std::string loadSubFuncDefinition(std::string &decipherJs, std::string &subFuncName);
        void extractSubFuncNames(std::string &subFuncDefinition);
        void extractDecipher(std::string& decipherFuncDefinition);

        void subReverse(std::string& a, int b);
        void subSplice(std::string& a, int b);
        void subSwap(std::string& a, int b);

        std::string subReverseName;
        std::string subSpliceName;
        std::string subSwapName;

        std::vector<std::tuple<std::string, int>> decipher;
    };
}  // namespace Youtube

#endif  //YOUTUBE_LIST_DOWNLOADER_DECIPHER_H
