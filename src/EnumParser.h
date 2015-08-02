#ifndef ENUMPARSER_H
#define ENUMPARSER_H

#include <string>
#include <Magick++.h>

template <typename T>
class EnumParser
{
    typename std::map <std::string, T> enumMap;
    public:
        EnumParser(){};

        T parse(const std::string &value)
        { 
            typename std::map <std::string, T>::const_iterator iValue = enumMap.find(value);
            if (iValue  == enumMap.end())
                throw;
            return iValue->second;
        }
};
#endif