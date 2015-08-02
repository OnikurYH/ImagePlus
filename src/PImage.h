#ifndef PIMAGE_H
#define PIMAGE_H

#include <string>
#include <Magick++.h>
#include <nan.h>

#include "IPUtil.h"
#include "EnumParser.h"

using namespace Nan;

class PImage : public ObjectWrap
{
    public:
        static NAN_MODULE_INIT(Init);

    private:
        PImage(int width, int height, int red, int green, int blue);
        PImage(int width, int height, std::string hexBackgroundColor);
        PImage(char* imgBuffer, size_t bufSize);
        ~PImage();

        static Persistent<v8::Function> constructor;
        static NAN_METHOD(New);
        
        static NAN_METHOD(resize);
        static NAN_METHOD(crop);
        static NAN_METHOD(composite);
        static NAN_METHOD(blur);
        static NAN_METHOD(shadow);
        static NAN_METHOD(setBackgroundColor);
        static NAN_METHOD(setFilterType);
        static NAN_METHOD(setType);
        static NAN_METHOD(toBuffer);
        static NAN_METHOD(toFile);
        
        Magick::Image magickImg_;
};
#endif