#ifndef ZHWK_HTTP_CONFIG_H
#define ZHWK_HTTP_CONFIG_H

#include <string>
#include <unordered_map>


// Configuration File Format:
// --------------------------
// Listen Address:Listen Port
// Wildcard Webroot
// Host(Fulltext Match) +
// Correspond Webroot   + xN

class Configurator{
    public:
    std::string addrxport;
    std::string wc_webroot;
    std::unordered_map<std::string, std::string> webroots;
};

int ReadConfig(std::string filepath, Configurator& conf);

extern Configurator cfg;


#endif
