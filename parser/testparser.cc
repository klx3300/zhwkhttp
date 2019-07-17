#include "parser.h"
#include <iostream>
#include <vector>

using namespace std;

const string HDRSTRING = "GET /fuckcnlabs.pr?a=fuck_you&b=suck HTTP/1.1\r\nHost: fuck.com\r\nUser-Agent: TestHdrGenerator/0.0.0.0\r\nAccept: */*\r\n\r\n";
const string PATSTR = "<>\nINLINE\n/fuck/fuckyou.html\n(a=b;b=c;c=d)(e=f)\n<>\nREDIRECT\n/fuck/aaa.html\n()";

class StringTokenizer:public TokenStream {
    public:
        StringTokenizer(const string& cstr);
        virtual int nextChar(char& c);
        string istr;
        size_t iterated;
};

StringTokenizer::StringTokenizer(const string& cstr){
    istr = cstr;
    iterated = 0;
}

int StringTokenizer::nextChar(char& c){
    if(iterated >= istr.length()) return -1;
    c = istr.c_str()[iterated];
    iterated ++;
    return 0;
}

int main(void){
    StringTokenizer strtoken(HDRSTRING);
    HttpHeader rethdr;
    int rv = parseHeader(strtoken, rethdr);
    cout << (rv ? "Parser: FAIL" : "Parser: OK") << endl;
    cout << " <-- HTTP HEADERS --> " << endl;
    cout << "Operation: [" << rethdr.requestType << "]" << endl;
    cout << "URI: [" << rethdr.uri << "]" << endl;
    cout << "Version: [" << rethdr.version << "]" << endl;
    cout << " <-- KV PAIRS --> " << endl;
    for(auto& rng : rethdr.params){
        cout << "[" << rng.first << "] => [" << rng.second << "]" << endl;
    }
    cout << " <-- DATA PAIRS --> " << endl;
    for(auto& rng : rethdr.data){
        cout << "[" << rng.first << "] => [" << rng.second << "]" << endl;
    }
    cout << "======================================================================" << endl;
    vector<PatternMatcher> vpm;
    StringTokenizer patToks(PATSTR);
    rv = parsePattern(patToks, vpm);
    cout << (rv ? "Parser: FAIL" : "Parser: OK") << endl;
    for(auto& pat: vpm){
        cout << " <-- Normal Params --> " << endl;
        cout << "Mode: [" << pat.mode << "]" << endl;
        cout << "URI: [" << pat.uri << "]" << endl;
        cout << " <-- Patterns --> " << endl;
        if(pat.pattern.size() == 0){
            cout << "Wildcard Pattern!!" << endl;
        }else{
            for(auto& eachMatch: pat.pattern){
                for(auto& matchp: eachMatch){
                    cout << matchp.first << " = " << matchp.second << "; ";
                }
                cout << endl;
            }
        }
    }
    return rv;
}
