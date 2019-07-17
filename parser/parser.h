#ifndef ZHWK_HTTP_PARSER_H
#define ZHWK_HTTP_PARSER_H

#include <string>
#include <map>
#include <vector>
#include <utility>

class TokenStream {
    public:
        virtual int nextChar(char& tok) = 0;
};

class HttpHeader {
    public:
        std::string requestType;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> params;
        std::map<std::string, std::string> data;
};

class ResponseHeader {
    public:
        std::string version; // "1.1"
        std::string statCode; // "200 OK"
        std::map<std::string, std::string> params;
        std::string payload;
};

enum PARSE_RETCODE {
    PARSE_OK = 0,
    PARSE_FAIL = -1
};

struct PatternMatcher {
    std::string mode; // "INLINE" or "REDIRECT"
    std::string uri;
    std::vector<std::map<std::string, std::string>> pattern;
};

const bool STRICT_MODE = true;

int parseHeader(TokenStream& toks, HttpHeader& hdr);
std::string asHeader(const ResponseHeader& hdr);

int parsePattern(TokenStream& toks, std::vector<PatternMatcher>& patterns);

#endif
