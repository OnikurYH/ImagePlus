#ifndef PIMAGE_H
#define PIMAGE_H

#include <string>
#include <Magick++.h>
#include <nan.h>

#include "IPUtil.h"
#include "EnumParser.h"

class PImage : public Nan::ObjectWrap
{
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        PImage(int width, int height);
        PImage(int width, int height, int red, int green, int blue);
        PImage(int width, int height, std::string hexBackgroundColor);
        PImage(char* imgBuffer, size_t bufSize);
        ~PImage();

        static void New         (const Nan::FunctionCallbackInfo<v8::Value>& info);

        static void resize      (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void crop        (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void composite   (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void blur        (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void shadow      (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void fillImage   (const Nan::FunctionCallbackInfo<v8::Value>& info);

        static void setStrokeColor    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setStrokeWidth    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setFillColor    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setFont    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setFontSize    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void getFontMetrics (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void drawText    (const Nan::FunctionCallbackInfo<v8::Value>& info);

        static void outlineImage (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setBackgroundColor (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setFilterType (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void setType     (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void clone       (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void toBuffer    (const Nan::FunctionCallbackInfo<v8::Value>& info);
        static void toFile      (const Nan::FunctionCallbackInfo<v8::Value>& info);

        static Nan::Persistent<v8::Function> constructor;

        Magick::Image *mImage;
};
#endif
