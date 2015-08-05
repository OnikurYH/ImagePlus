#include <Magick++.h>
#include <nan.h>

#include "PImage.h"

using namespace Nan;

NAN_MODULE_INIT(InitAll) {
    PImage::Init(target);
    Magick::InitializeMagick("");
}

NODE_MODULE(imageplus, InitAll);
