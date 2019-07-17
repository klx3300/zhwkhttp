#include "parser.h"
extern "C" {
    #include "../zhwkre/log.h"
}

using namespace std;

enum HTTP_STATUS {
    STATE_TYPE = 0,
    STATE_VERSION,
    STATE_PARAM_KEY,
    STATE_PARAM_VALUE,
    STATE_URI,
    STATE_URI_KEY,
    STATE_URI_VALUE
};

enum PATT_STATUS {
    PATT_DIRECTORY = 0,
    PATT_MODE,
    PATT_URI,
    PATT_PBEGIN,
    PATT_PKEY,
    PATT_PVAL
};

static bool lexical(char i){
    if(i == '\r') return false;
    if(i == '\n') return false;
    return true;
}

static bool nonblank(char i){
    if(i == '\t') return false;
    if(i == ' ') return false;
    return lexical(i);
}

static bool proper_key(char i){
    if(i == ':') return false;
    return nonblank(i);
}

static bool data_kv(char i){
    if(i == '=') return false;
    if(i == '&') return false;
    return nonblank(i);
}

static bool pattern_lex(char i){
    if(i == '=') return false;
    if(i == ';') return false;
    if(i == '(') return false;
    if(i == ')') return false;
    return nonblank(i);
}

static int expect(TokenStream& toks, string expstr){
    size_t i = 0;
    while(i < expstr.length()){
        char currtok = 0;
        if(toks.nextChar(currtok) == -1) return -1;
        if(expstr.c_str()[i] != currtok) return -1;
        i ++;
    }
    return 0;
}

int parseHeader(TokenStream& toks, HttpHeader& hdr){
    int state = STATE_TYPE;
    string tmpkey, tmpvalue;
    while(true){
        char currtok = 0;
        if(toks.nextChar(currtok) == -1){
            // read next token failed.
            // GG
            return PARSE_FAIL;
        }
        switch(state) {
            case STATE_TYPE:
                {
                    if(nonblank(currtok)){
                        hdr.requestType += currtok;
                    }else{
                        if(currtok != ' '){
                            // GG
                            qLogWarn("The thing after HTTP request type is not OK!");
                            return PARSE_FAIL;
                        }
                        state = STATE_URI;
                    }
                }
                break;
            case STATE_URI:
                {
                    if(nonblank(currtok) && currtok != '?'){
                        hdr.uri += currtok;
                    }else{
                        if(currtok != '?'){
                            if(currtok != ' ' || expect(toks, "HTTP/")){
                                qLogWarn("The thing after HTTP request URI is not OK!");
                                return PARSE_FAIL;
                            }
                        }
                        if(hdr.uri == "/") hdr.uri = "/index.html";
                        if(currtok == '?'){
                            state = STATE_URI_KEY;
                        } else {
                            state = STATE_VERSION;
                        }
                    }
                }
                break;
            case STATE_URI_KEY:
                {
                    if(data_kv(currtok)){
                        tmpkey += currtok;
                    } else {
                        if(currtok == '='){
                            if(tmpkey == ""){
                                qLogWarn("Embedded GET data has an empty key.");
                                return PARSE_FAIL;
                            }
                            state = STATE_URI_VALUE;
                        } else {
                            // false termination
                            qLogWarn("Unexpected termination of embedded GET data key.");
                            return PARSE_FAIL;
                        }
                    }
                }
                break;
            case STATE_URI_VALUE:
                {
                    if(data_kv(currtok)){
                        tmpvalue += currtok;
                    } else {
                        if(currtok == '&'){
                            if(tmpvalue == ""){
                                qLogWarn("Embedded GET data has an empty value.");
                                return PARSE_FAIL;
                            }
                            hdr.data[tmpkey] = tmpvalue;
                            tmpkey = "";
                            tmpvalue = "";
                            state = STATE_URI_KEY;
                        } else {
                            // could be..
                            if(currtok != ' ' || expect(toks, "HTTP/")){
                                qLogWarn("The thing after HTTP request URI data is not OK!");
                                return PARSE_FAIL;
                            }
                            if(tmpkey != "" && tmpvalue != ""){
                                hdr.data[tmpkey] = tmpvalue;
                                tmpkey = "";
                                tmpvalue = "";
                            }
                            state = STATE_VERSION;
                        }
                    }
                }
                break;
            case STATE_VERSION:
                {
                    if(nonblank(currtok)){
                        hdr.version += currtok;
                    }else{
                        if(currtok != '\r' || expect(toks, "\n")){
                            // GG
                            qLogWarn("The thing after HTTP version is not OK!");
                            return PARSE_FAIL;
                        }
                        state = STATE_PARAM_KEY;
                    }
                }
                break;
            case STATE_PARAM_KEY:
                {
                    if(proper_key(currtok)){
                        tmpkey += currtok;
                    } else if(currtok == ':'){
                        if(STRICT_MODE && expect(toks, " ")){
                            qLogWarn("HTTP Header kv pair malformed.");
                            return PARSE_FAIL;
                        }
                        state = STATE_PARAM_VALUE;
                    } else if(tmpkey =="" && currtok == '\r') {
                        if(expect(toks, "\n")){
                            qLogWarn("HTTP Header Terminage Sequence malformed.");
                            return PARSE_FAIL;
                        }
                        return PARSE_OK;
                    } else {
                        // inproper!!
                        qLogWarnfmt("HTTP Header Key[%s] reached unexpected termination!!", tmpkey.c_str());
                        return -1;
                    }
                }
                break;
            case STATE_PARAM_VALUE:
                {
                    // this accept anything that isn't line breaks
                    if(lexical(currtok)){
                        tmpvalue += currtok;
                    } else {
                        if(expect(toks, "\n")){
                            qLogWarnfmt("HTTP Header kv pair <%s: %s> terminage sequence malformed.", tmpkey.c_str(), tmpvalue.c_str());
                            return PARSE_FAIL;
                        }
                        // gonna save that
                        hdr.params[tmpkey] = tmpvalue;
                        tmpkey = "";
                        tmpvalue = "";
                        state = STATE_PARAM_KEY;
                    }
                }
                break;
            default:
                {
                    qLogWarn("Internal Control Sequence GG. CONTACT DEVELOPER!!");
                    return -1;
                }
                break;
        }
    }
}

string asHeader(const ResponseHeader& hdr){
    string headerStr = "";
    headerStr += "HTTP/" + hdr.version + " " + hdr.statCode + "\r\n";
    for(auto& pariter: hdr.params){
        headerStr += pariter.first + ": " + pariter.second + "\r\n";
    }
    headerStr += "\r\n" + hdr.payload;
    return headerStr;
}

int parsePattern(TokenStream& toks, vector<PatternMatcher>& patterns){
    int state = PATT_DIRECTORY;
    PatternMatcher tmp;
    string keybuf, valbuf;
    string modebuf, uribuf;
    map<string, string> tmpmap;
    while(true){
        char currtok = 0;
        if(toks.nextChar(currtok) == -1){
            // read next token failed.
            // this may means we already exhausted the input..
            if(state == PATT_PBEGIN) {
                // everything at a stable point, we think this indicates exhaustion
                patterns.push_back(tmp);
                return PARSE_OK;
            }
            return PARSE_FAIL;
        }
        switch(state){
            case PATT_DIRECTORY:{
                if(!nonblank(currtok)) continue;
                if(currtok == '<' && !expect(toks, ">")){
                    state = PATT_MODE;
                    continue;
                }
                qLogWarn("PatternMatcher: no good directory");
                return PARSE_FAIL;
            }break;
            case PATT_MODE:{
                if(!nonblank(currtok)){
                    if(modebuf != ""){
                        tmp.mode = modebuf;
                        modebuf = "";
                        state = PATT_URI;
                        continue;
                    }
                    continue;
                }
                modebuf += currtok;
            }break;
            case PATT_URI:{
                if(!nonblank(currtok)){
                    if(uribuf != ""){
                        tmp.uri = uribuf;
                        uribuf = "";
                        state = PATT_PBEGIN;
                        continue;
                    }
                    continue;
                }
                uribuf += currtok;
            }break;
            case PATT_PBEGIN:{
                if(!nonblank(currtok)) continue;
                if(currtok != '(' && currtok != '<') {
                    qLogWarnfmt("PatternMatcher: garbage [%hhu, %c] between previous element and another pattern", currtok, currtok);
                    return PARSE_FAIL;
                } else if(currtok == '<' && !expect(toks, ">")){
                    patterns.push_back(tmp);
                    tmp = PatternMatcher();
                    state = PATT_MODE;
                } else {
                    keybuf = "";
                    valbuf = "";
                    state = PATT_PKEY;
                }
            }break;
            case PATT_PKEY:{
                if(pattern_lex(currtok)){
                    keybuf += currtok;
                    continue;
                }
                if(currtok == ')'){
                    if(keybuf == ""){
                        // empty pattern, means wildcard
                        state = PATT_PBEGIN; // issue next parsing
                        continue;
                    }
                    qLogWarnfmt("PatternMatcher: key %s doesn't have an value", keybuf.c_str());
                    return PARSE_FAIL;
                }
                if(currtok == '='){
                    state = PATT_PVAL;
                    continue;
                }
                qLogWarnfmt("PatternMatcher: garbage followed after key [%s]", keybuf.c_str());
                return PARSE_FAIL;
            }break;
            case PATT_PVAL:{
                if(pattern_lex(currtok)){
                    valbuf += currtok;
                    continue;
                }
                if(currtok == ';'){
                    tmpmap[keybuf] = valbuf;
                    keybuf = "";
                    valbuf = "";
                    state = PATT_PKEY;
                    continue;
                }
                if(currtok == ')'){
                    // patt ok
                    tmpmap[keybuf] = valbuf;
                    keybuf = "";
                    valbuf = "";
                    tmp.pattern.push_back(tmpmap);
                    tmpmap = map<string, string>();
                    state = PATT_PBEGIN;
                    continue;
                }
                qLogWarnfmt("PatternMatcher: garbage after KV [%s => %s]", keybuf.c_str(), valbuf.c_str());
                return PARSE_FAIL;
            }break;
            default:
                qLogFail("PatternMatcher: Internal logic is in indefinite state, CONTACT DEVELOPER!!");
                abort();
            break;
        }
    }
}

