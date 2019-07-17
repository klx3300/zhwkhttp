#include "config/config.h"
#include "parser/parser.h"
#include "stroper/stroper.h"
#include "task/task.h"

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>

extern "C"{
#include "zhwkre/network.h"
#include "zhwkre/log.h"
#include <errno.h>
#include <signal.h>
}

using namespace std;

const string CONF_PATH = "./config";

qSocket listener;

void closesrv(int sig) {
    qLogInfo("Server: Exiting..");
    qSocket_close(listener);
    exit(0);
}

void ign(int sig){
    qLogInfo("Handler: Connection Broken");
}

void listenT(){
    int hid = 0;
    while(true){
        char srcaddr[100] = {0};
        qSocket cliconn = qStreamSocket_accept(listener, srcaddr);
        if(cliconn.desc == -1){
            qLogInfofmt("Service: accept: %s", strerror(errno));
            return;
        }
        string identity = srcaddr;
        thread hth(RequestHandler, cliconn, identity, hid);
        hth.detach();
        hid ++;
    }
}

int main(int argc, char** argv){
    // initialize configurations
    int rcv = ReadConfig(CONF_PATH, cfg);
    if(rcv == -1){
        return 1;
    }
    signal(SIGINT, closesrv);
    signal(SIGPIPE, ign);
    string comm;
    while(getline(cin, comm)){
        if(comm == "s"){
            // construct listen socket
            qLogInfo("Server: Starting..");
            listener = qSocket_constructor(qIPv4, qStreamSocket, qDefaultProto);
            int oprv = qSocket_open(listener);
            if(oprv == -1){
                qLogFailfmt("Main: Cannot open listener socket: %s", strerror(errno));
                continue;
            }
            int enabled = 1;
            setsockopt(listener.desc, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));
            enabled = 1;
            setsockopt(listener.desc, SOL_SOCKET, SO_REUSEPORT, &enabled, sizeof(enabled));
            int bdrv = qSocket_bind(listener, cfg.addrxport.c_str());
            if(bdrv == -1){
                qLogFailfmt("Main: Cannot bind listener socket to %s: %s",
                        cfg.addrxport.c_str(), strerror(errno));
                qSocket_close(listener);
                continue;
            }
            int lsrv = qStreamSocket_listen(listener);
            if(lsrv == -1){
                qLogFailfmt("Main: Cannot listen: %s", strerror(errno));
                qSocket_close(listener);
                continue;
            }
            // start ACCEPT!!
            thread lt(listenT);
            lt.detach();
        } else if (comm == "q") {
            qLogInfo("Server: Exiting..");
            qSocket_close(listener);
        }
    }
    qLogInfo("Server: Exiting..");
    qSocket_close(listener);
    return 0;
}
