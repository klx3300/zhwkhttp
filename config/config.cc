#include "config.h"
#include <fstream>
extern "C"{
#include "../zhwkre/log.h"
}

using namespace std;

Configurator cfg;

int ReadConfig(string fpath, Configurator& cfg){
    fstream fs(fpath, ios_base::in);
    string linebuffer;
    string dombuffer;
    int lineno = 0;
    while(getline(fs, linebuffer)){
        if(lineno == 0){
            cfg.addrxport = linebuffer;
            qLogInfofmt("Configurator: Listen address %s", linebuffer.c_str());
        } else if (lineno == 1){
            cfg.wc_webroot = linebuffer;
            qLogInfofmt("Configurator: Wildcard Webroot %s", linebuffer.c_str());
        } else if (lineno % 2 == 0){
            dombuffer = linebuffer;
        } else {
            cfg.webroots[dombuffer] = linebuffer;
            qLogInfofmt("Configurator: Mapping %s => %s", dombuffer.c_str(), linebuffer.c_str());
        }
        lineno ++;
    }
    if(lineno < 1){
        qLogFail("Configurator: Insufficient configuration content.");
    }
    return 0;
}
