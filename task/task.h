#ifndef ZHWK_HTTP_TASK_H
#define ZHWK_HTTP_TASK_H

extern "C"{
#include "../zhwkre/network.h"
}

#include <string>
#include "../parser/parser.h"

class SocketStream: public TokenStream {
    public:
    qSocket sock;
    virtual int nextChar(char& tok);
};

class FileStream: public TokenStream {
    public:
    FILE* fp;
    virtual int nextChar(char& tok);
};

void RequestHandler(qSocket sock, std::string identity, int handid);

#endif
