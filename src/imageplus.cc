#include <Magick++.h>
#include <nan.h>

#include "PImage.h"

using namespace Nan;

void InitAll(v8::Local<v8::Object> exports) {
    PImage::Init(exports);
    Magick::InitializeMagick("");
}

NODE_MODULE(imageplus, InitAll);
