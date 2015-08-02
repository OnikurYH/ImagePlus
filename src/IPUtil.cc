#include "IPUtil.h"

#include <iostream>

RGB IPUtil::hexStringToRGB (std::string strHex)
{
    int hexColor = std::strtol (strHex.c_str(), NULL, 16);
    struct RGB colorRGB;
    colorRGB.r = ((hexColor >> 16) & 0xFF) / 255.0;
    colorRGB.g = ((hexColor >> 8) & 0xFF) / 255.0;
    colorRGB.b = ((hexColor) & 0xFF) / 255.0;
    
    std::cout << colorRGB.r << " " << colorRGB.g << " " << colorRGB.b << std::endl;
    
    return colorRGB;
}