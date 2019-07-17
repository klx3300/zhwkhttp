#include "task.h"
#include "../stroper/stroper.h"
#include "../config/config.h"
#include <cstring>

extern "C" {
#include "../zhwkre/log.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
}

using namespace std;

const string SERVER_STR = "zhwkhttp/whatever";
const string DEF_CACHE_CTRL = "private, no-cache, no-store, proxy-revalidate, no-transform";

const string ERR_404 = string(
    "<html>\n<head>\n<title>404 Not Found</title>\n</head>"
    "<body bgcolor=\"white\">"
    "<center><h1>404 Not Found</h1></center>"
    "<hr><center>zhwkhttp/What is reasonable may exist, but not vise versa.</center>"
    "</body>\n</html>"
    );

const string ERR_501 = string(
    "<html>\n<head>\n<title>501 Not Implemented</title>\n</head>"
    "<body bgcolor=\"white\">"
    "<center><h1>501 Not Implemented</h1></center>"
    "<hr><center>zhwkhttp/You may consider supporting the author for advanced functionalities.</center>"
    "</body>\n</html>"
    );

const string ERR_500 = string(
    "<html>\n<head>\n<title>500 Internal Server Error</title>\n</head>"
    "<body bgcolor=\"white\">"
    "<center><h1>500 Internal Server Error</h1></center>"
    "<hr><center>zhwkhttp/Do You Know: You may not want this thing so desparately..</center>"
    "</body>\n</html>"
    );

const string ERR_502 = string(
    "<html>\n<head>\n<title>502 Bad Gateway</title>\n</head>"
    "<body bgcolor=\"white\">"
    "<center><h1>502 Bad Gateway</h1></center>"
    "<hr><center>zhwkhttp/Convince me that you are modern human..</center>"
    "</body>\n</html>"
    );

int SocketStream::nextChar(char& tok){
    char fk = '\0';
    int rv = qStreamSocket_readchar(sock, &fk);
    tok = fk;
    return rv;
}

int FileStream::nextChar(char& tok){
    int rv = fgetc(fp);
    if(rv == EOF) return -1;
    tok = (char)rv;
    return rv;
}

void RequestHandler_307(qSocket sock, string identity, int handid, string redir){
    qLogInfofmt("RequestHandler[%d]: 307 Internal Redirect.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "307 Internal Redirect";
    resp.params["Location"] = redir;
    // response formed, send to client..!!
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
    qSocket_close(sock);
    return;
}

void RequestHandler_502(qSocket sock, string identity, int handid){
    qLogInfofmt("RequestHandler[%d]: 502 Bad Gateway.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "502 Bad Gateway";
    resp.params["Content-Length"] = ItoS((int)ERR_502.size());
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = "text/html";
    resp.params["Connection"] = "close";
    resp.params["Date"] = DtoS(time(0));
    resp.params["Cache-Control"] = DEF_CACHE_CTRL;
    resp.payload = ERR_502;
    // response formed, send to client..!!
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
    qSocket_close(sock);
    return;
}

void RequestHandler_501(qSocket sock, string identity, int handid){
    qLogInfofmt("RequestHandler[%d]: 501 Not Implemented.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "501 Not Implemented";
    resp.params["Content-Length"] = ItoS((int)ERR_501.size());
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = "text/html";
    resp.params["Connection"] = "close";
    resp.params["Date"] = DtoS(time(0));
    resp.params["Cache-Control"] = DEF_CACHE_CTRL;
    resp.payload = ERR_501;
    // response formed, send to client..!!
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
    qSocket_close(sock);
    return;
}

void RequestHandler_500(qSocket sock, string identity, int handid){
    qLogInfofmt("RequestHandler[%d]: 500 Internal Server Error.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "500 Internal Server Error";
    resp.params["Content-Length"] = ItoS((int)ERR_500.size());
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = "text/html";
    resp.params["Connection"] = "close";
    resp.params["Date"] = DtoS(time(0));
    resp.params["Cache-Control"] = DEF_CACHE_CTRL;
    resp.payload = ERR_500;
    // response formed, send to client..!!
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
    qSocket_close(sock);
    return;
}
void RequestHandler_404(qSocket sock, string identity, int handid){
    qLogInfofmt("RequestHandler[%d]: 404 Not Found.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "404 Not Found";
    resp.params["Content-Length"] = ItoS((int)ERR_404.size());
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = "text/html";
    resp.params["Connection"] = "close";
    resp.params["Date"] = DtoS(time(0));
    resp.payload = ERR_404;
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
    qSocket_close(sock);
    return;
}

void RequestHandler_normalf(qSocket sock, string identity, int handid, string filepath, HttpHeader& srchdr, bool& kp_alive){
    struct stat reqst = {0};
    stat(filepath.c_str(), &reqst);
    char* filecont = reinterpret_cast<char*>(malloc(reqst.st_size));
    if(filecont == NULL){
        qLogWarnfmt("RequestHandler[%d]: Insufficient memory for %ld",
                handid, reqst.st_size);
        RequestHandler_500(sock, identity, handid);
        return;
    }
    int reqfd = open(filepath.c_str(), 0);
    if(reqfd == -1){
        qLogWarnfmt("RequestHandler[%d]: Cannot open %s: %s",
                handid, (filepath).c_str(), strerror(errno));
        free(filecont);
        RequestHandler_404(sock, identity, handid);
        return;
    }
    ssize_t rdsz = read(reqfd, filecont, reqst.st_size);
    if(rdsz != reqst.st_size){
        qLogWarnfmt("RequestHandler[%d]: Cannot read sufficient amount of content from file %s: %s",
                handid, (filepath).c_str(), strerror(errno));
        free(filecont);
        RequestHandler_500(sock, identity, handid);
        return;
    }
    // file content get!
    close(reqfd);
    qLogInfofmt("RequestHandler[%d]: 200 OK.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "200 OK";
    resp.params["Content-Length"] = ItoS((int)reqst.st_size);
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = guessMIME(filepath);
    resp.params["Date"] = DtoS(time(0));
    resp.params["Cache-Control"] = DEF_CACHE_CTRL;
    /* if(srchdr.params["Connection"] == "Keep-Alive" || */
    /*         srchdr.params["Connection"] == "keep-alive"){ */
    /*     // do the keep alive work */
    /*     resp.params["Connection"] = "keep-alive"; */
    /*     kp_alive = true; */
    /* } else { */
    /*     kp_alive = false; */
    /*     resp.params["Connection"] = "close"; */
    resp.params["Connection"] = "close";
    /* } */
    // inject payload
    resp.payload = string(filecont, reqst.st_size);
    free(filecont);
    // OK, return to client
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
}

void RequestHandler_pattern(qSocket sock, string identity, int handid, string filepath, HttpHeader& srchdr, bool& kp_alive, string& webroot){
    FILE* pattfp = fopen(filepath.c_str(), "r");
    FileStream ftoks;
    ftoks.fp = pattfp;
    vector<PatternMatcher> vpm;
    int pv = parsePattern(ftoks, vpm);
    if(pv != PARSE_OK){
        qLogWarnfmt("RequestHandler[%d]: the pattern file is malformed.", handid);
        RequestHandler_500(sock, identity, handid);
        kp_alive = false;
        return;
    }
    bool is_redir = false, is_matched = false;;
    string real_path;
    // do pattern matching work
    for(auto& pattMatcher: vpm){
        if(pattMatcher.pattern.size() == 0){
            // Wildcard
            is_redir = (pattMatcher.mode == "REDIRECT");
            real_path = pattMatcher.uri;
            is_matched = true;
            break;
        }
        for(auto& pattMap: pattMatcher.pattern){
            bool check_ok = true;
            for(auto& pattern: pattMap){
                if(pattern.second != srchdr.data[pattern.first]){
                    check_ok = false;
                    break;
                }
            }
            if(check_ok){
                // matched
                is_redir = (pattMatcher.mode == "REDIRECT");
                real_path = pattMatcher.uri;
                is_matched = true;
                break;
            }
        }
        if(is_matched) break;
    }
    if(!is_matched){
        qLogWarnfmt("RequestHandler[%d]: nothing matched.", handid);
        RequestHandler_502(sock, identity, handid);
        kp_alive = false;
        return;
    }
    if(is_redir){
        RequestHandler_307(sock, identity, handid, real_path);
        kp_alive = false;
        return;
    }
    real_path = webroot + real_path;
    if(access(real_path.c_str(), R_OK)){
        qLogInfofmt("RequestHandler[%d]: Request from %s want to access %s, which isn't exist or have insufficient permission",
                handid, identity.c_str(), (real_path).c_str());
        RequestHandler_404(sock, identity, handid);
        return;
    }
    struct stat reqst = {0};
    stat(real_path.c_str(), &reqst);
    char* filecont = reinterpret_cast<char*>(malloc(reqst.st_size));
    if(filecont == NULL){
        qLogWarnfmt("RequestHandler[%d]: Insufficient memory for %ld",
                handid, reqst.st_size);
        RequestHandler_500(sock, identity, handid);
        return;
    }
    int reqfd = open(real_path.c_str(), O_RDONLY);
    if(reqfd == -1){
        qLogWarnfmt("RequestHandler[%d]: Cannot open %s: %s",
                handid, (real_path).c_str(), strerror(errno));
        free(filecont);
        RequestHandler_404(sock, identity, handid);
        return;
    }
    ssize_t rdsz = read(reqfd, filecont, reqst.st_size);
    if(rdsz != reqst.st_size){
        qLogWarnfmt("RequestHandler[%d]: Cannot read sufficient amount of content from file %s: %s",
                handid, (real_path).c_str(), strerror(errno));
        free(filecont);
        RequestHandler_500(sock, identity, handid);
        return;
    }
    // file content get!
    close(reqfd);
    qLogInfofmt("RequestHandler[%d]: 200 OK.", handid);
    ResponseHeader resp;
    resp.version = "1.1";
    resp.statCode = "200 OK";
    resp.params["Content-Length"] = ItoS((int)reqst.st_size);
    resp.params["Server"] = SERVER_STR;
    resp.params["Content-Type"] = guessMIME(real_path);
    resp.params["Date"] = DtoS(time(0));
    resp.params["Cache-Control"] = DEF_CACHE_CTRL;
    /* if(srchdr.params["Connection"] == "Keep-Alive" || */
    /*         srchdr.params["Connection"] == "keep-alive"){ */
    /*     // do the keep alive work */
    /*     resp.params["Connection"] = "keep-alive"; */
    /*     kp_alive = true; */
    /* } else { */
    /*     kp_alive = false; */
    /*     resp.params["Connection"] = "close"; */
    resp.params["Connection"] = "close";
    /* } */
    // inject payload
    resp.payload = string(filecont, reqst.st_size);
    free(filecont);
    // OK, return to client
    string rs = asHeader(resp);
    qStreamSocket_write(sock, rs.c_str(), rs.length());
}

void RequestHandler(qSocket sock, string identity, int handid){
    qLogDebugfmt("RequestHandler[%d]: Begin handling request from %s", handid, identity.c_str());
    bool kp_alive = false;
    SocketStream ss;
    ss.sock = sock;
    {
        kp_alive = false;
        HttpHeader srchdr;
        // read request head
        int parserv = parseHeader(ss, srchdr);
        if(parserv != 0){
            qLogWarnfmt("RequestHandler[%d]: Request from %s contains illegal content.",
                    handid, identity.c_str());
            RequestHandler_501(sock, identity, handid);
            return;
        }
        // check http versions
        if(srchdr.version != "1.1"){
            qLogWarnfmt("RequestHandler[%d]: Request from %s implied unsupported HTTP version %s",
                    handid, identity.c_str(), srchdr.version.c_str());
            RequestHandler_501(sock, identity, handid);
            return;
        }
        qLogInfofmt("RequestHandler[%d]: HTTP/%s, %s -> %s", handid,
                srchdr.version.c_str(), srchdr.requestType.c_str(), srchdr.uri.c_str());
        if(srchdr.requestType == "GET"){
            // TODO: implement this
            string webroot = "";
            if(cfg.webroots.find(srchdr.params["Host"]) != cfg.webroots.end()){
                webroot = cfg.webroots[srchdr.params["Host"]];
            } else {
                webroot = cfg.wc_webroot;
            }
            string filepath = webroot + srchdr.uri;
            if(access(filepath.c_str(), R_OK)){
                qLogInfofmt("RequestHandler[%d]: Request from %s want to access %s, which isn't exist or have insufficient permission",
                        handid, identity.c_str(), (filepath).c_str());
                RequestHandler_404(sock, identity, handid);
                return;
            }
            if(getExtension(filepath) != "pattern"){
                RequestHandler_normalf(sock, identity, handid, filepath, srchdr, kp_alive);
            } else {
                RequestHandler_pattern(sock, identity, handid, filepath, srchdr, kp_alive, webroot);
            }
            kp_alive = false;
            
        } else {
            qLogWarnfmt("RequestHandler[%d]: Request from %s implied unsupported HTTP method %s",
                    handid, identity.c_str(), srchdr.requestType.c_str());
            RequestHandler_501(sock, identity, handid);
            return;
        }
    }
    qSocket_close(sock);
}
