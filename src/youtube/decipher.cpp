#include <algorithm>
#include <boost/regex.hpp>
#include "json/json.hpp"
#include "networking/networking.h"
#include "youtube/decipher.h"

using json = nlohmann::json;

void Youtube::Decipher::decipherSignature(std::string& signature) {
    for (const auto& sub : decipher) {
        const std::string& funcName = std::get<0>(sub);
        int funcArg = std::get<1>(sub);
        if (funcName == subReverseName) {
            subReverse(signature, funcArg);
        } else if (funcName == subSpliceName) {
            subSplice(signature, funcArg);
        } else if (funcName == subSwapName) {
            subSwap(signature, funcArg);
        }
    }
}

void Youtube::Decipher::loadDecipher(std::string& videoHtml) {
    std::string decipherJs = loadDecipherJS(videoHtml);
    std::string decipherFuncName = loadDecipherFuncName(decipherJs);
    std::string decipherFuncDefinition = loadDecipherFuncDefinition(decipherJs, decipherFuncName);
    std::string subFuncName = loadSubFuncName(decipherFuncDefinition);
    std::string subFuncDefinition = loadSubFuncDefinition(decipherJs, subFuncName);
    extractSubFuncNames(subFuncDefinition);
    extractDecipher(decipherFuncDefinition);
}

std::string Youtube::Decipher::loadDecipherJS(std::string& videoHtml) {
    //RegExes
    boost::regex exprVideoAssets{R"(ytplayer.config\s=\s{.*"assets":({.*?}))"};

    // Matches
    boost::smatch matchesAssets;

    // Search decipher JS
    boost::regex_search(videoHtml, matchesAssets, exprVideoAssets);
    std::string strDecipherJS = matchesAssets[1];

    json jsDecipherJS = json::parse(strDecipherJS);

    // Load video player JS
    Networking::Https::Response htmlResponse = Networking::Https::Get("s.ytimg.com", jsDecipherJS["js"], "*/*");

    return htmlResponse.body;
}

std::string Youtube::Decipher::loadDecipherFuncName(std::string& decipherJs) {
    //RegExes
    boost::regex exprDecipherFuncName{R"("signature",(\w\w))"};

    // Matches
    boost::smatch matchesDecipherFunc;

    // Search decipher function
    boost::regex_search(decipherJs, matchesDecipherFunc, exprDecipherFuncName);
    return matchesDecipherFunc[1];
}

std::string Youtube::Decipher::loadDecipherFuncDefinition(std::string& decipherJs, std::string& decipherFuncName) {
    // RegExes
    boost::regex exprSubFuncDefinition{decipherFuncName + R"(=function\(.+?\)\{(.+?)\})"};

    // Matches
    boost::smatch matchesSubFuncs;

    // Search decipher function definition
    boost::regex_search(decipherJs, matchesSubFuncs, exprSubFuncDefinition);
    return matchesSubFuncs[1];
}

std::string Youtube::Decipher::loadSubFuncName(std::string& decipherFuncDefinition) {
    //RegExes
    boost::regex exprSubFuncName{R"((..)\.(..)\(.,(\d)+\))"};

    // Matches
    boost::smatch matchesSubFunc;

    std::stringstream ss(decipherFuncDefinition);
    std::string item;
    while (std::getline(ss, item, ';')) {
        boost::regex_search(item, matchesSubFunc, exprSubFuncName);
        std::string strSubFuncNs = matchesSubFunc[1];

        if (!strSubFuncNs.empty())
            return strSubFuncNs;
    }
}

std::string Youtube::Decipher::loadSubFuncDefinition(std::string& decipherJs, std::string& subFuncName) {
    // SubFuncName may contain special chars like $
    std::string fixedSubFuncName;

    for(auto c : subFuncName){
        fixedSubFuncName.append(1, '\\');
        fixedSubFuncName.append(1, c);
    }

    std::string strRegex = R"(var\s)" + fixedSubFuncName + R"(={((?:\n|.)*?)};)";

    //RegExes
    boost::regex exprSubFuncDefinition{strRegex};

    // Matches
    boost::smatch matchesSubFuncDefinitions;

    boost::regex_search(decipherJs, matchesSubFuncDefinitions, exprSubFuncDefinition);
    return matchesSubFuncDefinitions[1];
}

void Youtube::Decipher::extractSubFuncNames(std::string& subFuncDefinition) {
    //RegExes
    boost::regex exprSubFuncNames{R"((\w\w):function\(.+?\){(.*?)})"};

    // Iterator ends
    boost::sregex_iterator iterEnd;

    //Qualities
    boost::sregex_iterator iterSubFuncNames(subFuncDefinition.begin(), subFuncDefinition.end(), exprSubFuncNames);

    while (iterSubFuncNames != iterEnd) {
        boost::smatch matchesSubFuncName = *iterSubFuncNames++;

        std::string strDef = matchesSubFuncName[2];

        if (strDef.find("reverse") != std::string::npos)
            subReverseName = matchesSubFuncName[1];
        else if (strDef.find("splice") != std::string::npos)
            subSpliceName = matchesSubFuncName[1];
        else
            subSwapName = matchesSubFuncName[1];
    }
}

void Youtube::Decipher::extractDecipher(std::string& decipherFuncDefinition) {
    // Create "decipher"
    // RegExes
    boost::regex exprSubFunc{R"(\.(..)\(.,(\d+)\))"};

    // Matches
    boost::smatch matchesSubFunc;

    std::stringstream ss(decipherFuncDefinition);
    std::string item;
    while (std::getline(ss, item, ';')) {
        if (!boost::regex_search(item, matchesSubFunc, exprSubFunc))
            continue;

        std::string funcName = matchesSubFunc[1];
        std::string funcArgTest = matchesSubFunc[2];
        int funcArg = stoi(matchesSubFunc[2]);

        decipher.emplace_back(funcName, funcArg);
    }
}

void Youtube::Decipher::subReverse(std::string& a, int b) {
    std::reverse(a.begin(), a.end());
}

void Youtube::Decipher::subSplice(std::string& a, int b) {
    a.erase(0, static_cast<unsigned long long int>(b));
}

void Youtube::Decipher::subSwap(std::string& a, int b) {
    char c = a[0];
    a[0] = a[b % a.length()];
    a[b] = c;
}
