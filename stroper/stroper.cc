#include "stroper.h"
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <ctime>

using namespace std;

bool str_startwith(string src,string chk){
    return !src.find_first_of(chk);
}

bool str_endwith(string src,string chk){
    return src.find_last_of(chk) == (src.size()-chk.size());
}

string str_base_trim(string src,string trimer){
    string tmpstr;
    string bufferstr;
    int state = 0;
    for(auto ch:src){
        for(auto chk:trimer){
            if(ch == chk){
                if(state == 0) state = 1;
                else state = 3;
                break;
            }
        }
        switch(state){
            case 0:
                tmpstr += ch;
                state = 2;
                break;
            case 1:
                state = 0;
                break;
            case 2:
                tmpstr += bufferstr;
                tmpstr += ch;
                bufferstr = "";
                break;
            case 3:
                bufferstr += ch;
                state = 2;
                break;
            default:
                abort();
        }
    }
    return tmpstr;
}

string str_trim(string src){
    return str_base_trim(src," ");
}

string ItoS(int i) {
    char tmp[40] = {0};
    sprintf(tmp, "%d", i);
    return string(tmp);
}

string DtoS(time_t extime){
    char buf[1000];
    time_t now = extime;
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return string(buf);
}

string getExtension(string relpath){
    return relpath.substr(relpath.find_last_of('.') + 1, string::npos);
}

string guessMIME(string relpath){
    string ext = getExtension(relpath);
    if(ext == "htm" || ext == "html"){
        return "text/html; charset=utf-8";
    }
    if(ext == "js"){
        return "application/javascript; charset=utf-8";
    }
    if(ext == "css"){
        return "text/css; charset=utf-8";
    }
    if(ext == "jpg" || ext == "jpeg"){
        return "image/jpeg";
    }
    if(ext == "png"){
        return "image/png";
    }
    if(ext == "gif"){
        return "image/gif";
    }
    if(ext == "mp4"){
        return "video/mp4";
    }
    return "text/plain";
}



