#ifndef IP_UTIL_H
#define IP_UTIL_H

#include <string>
#include <cstdlib>

struct RGB
{
    int r;
    int g;
    int b;
};

class IPUtil
{
    public:
        static RGB hexStringToRGB (std::string str);
};

#endif