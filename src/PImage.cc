#include "PImage.h"

// Enums
// Magick FilterTypes
template <>
EnumParser<Magick::FilterTypes>::EnumParser()
{
    enumMap["Undefined"]        = Magick::UndefinedFilter;
    enumMap["Point"]            = Magick::PointFilter;
    enumMap["Box"]              = Magick::BoxFilter;
    enumMap["Triangle"]         = Magick::TriangleFilter;
    enumMap["Hermite"]          = Magick::HermiteFilter;
    enumMap["Hanning"]          = Magick::HanningFilter;
    enumMap["Hamming"]          = Magick::HammingFilter;
    enumMap["Blackman"]         = Magick::BlackmanFilter;
    enumMap["Gaussian"]         = Magick::GaussianFilter;
    enumMap["Quadratic"]        = Magick::QuadraticFilter;
    enumMap["Cubic"]            = Magick::CubicFilter;
    enumMap["Catrom"]           = Magick::CatromFilter;
    enumMap["Mitchell"]         = Magick::MitchellFilter;
    enumMap["Lanczos"]          = Magick::LanczosFilter;
    enumMap["Bessel"]           = Magick::BesselFilter;
    enumMap["Sinc"]             = Magick::SincFilter;
}
EnumParser<Magick::FilterTypes> magickFilterTypes;
// End Enums

Persistent<v8::Function> PImage::constructor;

PImage::PImage(int width, int height)
{
    Magick::Image magickImg (Magick::Geometry(width, height), Magick::Color(0, 0, 0, 0));
    magickImg_ = magickImg;
}

PImage::PImage(int width, int height, int red, int green, int blue)
{
    Magick::Image magickImg (Magick::Geometry(width, height), Magick::ColorRGB(red, green, blue));
    magickImg_ = magickImg;
}

PImage::PImage(int width, int height, std::string hexBackgroundColor)
{
    Magick::Image magickImg (Magick::Geometry(width, height), Magick::Color(hexBackgroundColor));
    magickImg_ = magickImg;
}

PImage::PImage(char* imgBuffer, size_t bufSize)
{
    Magick::Blob imgBlob (imgBuffer, bufSize);
    Magick::Image magickImg (imgBlob);
    magickImg_ = magickImg;
}

PImage::PImage(const PImage& otherPImage)
{
    magickImg_ = otherPImage.magickImg_;
}

PImage::~PImage()
{

}

NAN_MODULE_INIT(PImage::Init)
{
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(PImage::New);
    tpl->SetClassName(Nan::New("PImage").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "resize", resize);
    Nan::SetPrototypeMethod(tpl, "crop", crop);
    Nan::SetPrototypeMethod(tpl, "composite", composite);
    Nan::SetPrototypeMethod(tpl, "blur", blur);
    Nan::SetPrototypeMethod(tpl, "shadow", shadow);
    Nan::SetPrototypeMethod(tpl, "fillImage", fillImage);
    Nan::SetPrototypeMethod(tpl, "outlineImage", outlineImage);
    Nan::SetPrototypeMethod(tpl, "setBackgroundColor", setBackgroundColor);
    Nan::SetPrototypeMethod(tpl, "setFilterType", setFilterType);
    Nan::SetPrototypeMethod(tpl, "setType", setType);
    Nan::SetPrototypeMethod(tpl, "clone", clone);
    Nan::SetPrototypeMethod(tpl, "toBuffer", toBuffer);
    Nan::SetPrototypeMethod(tpl, "toFile", toFile);

    constructor.Reset(tpl->GetFunction());
    Nan::Set(target, Nan::New("PImage").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(PImage::New)
{
    if (info.IsConstructCall())
    {
        if (info.Length() == 1)
        {
            v8::Local<v8::Value> rawImgBuffer = info[0];

            if(!node::Buffer::HasInstance(rawImgBuffer))
            {
                Nan::ThrowError("Not a buffer!");
                info.GetReturnValue().SetUndefined();
            }

            char* imgBuffer = node::Buffer::Data(rawImgBuffer);
            size_t bufSize = node::Buffer::Length(rawImgBuffer);

            PImage* obj = new PImage(imgBuffer, bufSize);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }
        else if (info.Length() == 2)
        {
            if (info[0]->IsNumber() || info[1]->IsNumber())
            {
                int width = info[0]->NumberValue();
                int height = info[1]->NumberValue();

                PImage* obj = new PImage(width, height);
                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            }
            else
                Nan::ThrowError("Invalid argument!");
        }
        else if (info.Length() == 3)
        {
            if (info[0]->IsNumber() || info[1]->IsNumber() || info[2]->IsString())
            {
                int width = info[0]->NumberValue();
                int height = info[1]->NumberValue();
                v8::String::Utf8Value hexBackgroundColor (info[2]->ToString());

                PImage* obj = new PImage(width, height, std::string(*hexBackgroundColor));
                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            }
            else
                Nan::ThrowError("Invalid argument!");
        }
        else if (info.Length() == 5)
        {
            if (info[0]->IsNumber() || info[1]->IsNumber() || info[2]->IsString() || info[3]->IsString() || info[4]->IsString())
            {
                int width = info[0]->NumberValue();
                int height = info[1]->NumberValue();
                double red = info[2]->NumberValue();
                double green = info[3]->NumberValue();
                double blue = info[4]->NumberValue();

                PImage* obj = new PImage(width, height, red, green, blue);
                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            }
        }
        else
        {
            std::string strError = "Invalid argument! Available constructors:\n";
            strError.append("-    Image(Buffer image_buffer)\n");
            strError.append("-    Image(int width, int height, string hex_background_color)\n");
            strError.append("-    Image(int width, int height, int red, int green, int blue)\n");
            Nan::ThrowError(strError.c_str());
        }
    }
    else
    {
    	const int argc = 1;
    	v8::Local<v8::Value> argv[argc] = { info[0] };
    	v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    	info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

/**
 * Crop image
 * @param width New width
 * @param height New height
 */
NAN_METHOD(PImage::resize)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined()) ||
	(!info[0]->IsNumber() || !info[1]->IsNumber()))
    {
	Nan::ThrowError("Invalid argument!");
	info.GetReturnValue().SetUndefined();
    }

    int width = info[0]->NumberValue();
    int height = info[1]->NumberValue();

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.resize(Magick::Geometry(width, height));
}

/**
 * Crop image
 * @param x Position x
 * @param y Position y
 * @param width Width
 * @param height Height
 */
NAN_METHOD(PImage::crop)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined() || info[2]->IsUndefined() || info[3]->IsUndefined()) ||
        (!info[0]->IsNumber()   || !info[1]->IsNumber()   || !info[2]->IsNumber()   || !info[3]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    int x = info[0]->NumberValue();
    int y = info[1]->NumberValue();
    int width = info[2]->NumberValue();
    int height = info[3]->NumberValue();

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.chop(Magick::Geometry(x, y));
    pImage->magickImg_.crop(Magick::Geometry(width, height));
}

/**
 * Composite two image
 * @param imgOverlay The second image
 * @param x Position x
 * @param y Position y
 */
NAN_METHOD(PImage::composite)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined() || info[2]->IsUndefined()) ||
        (!info[0]->IsObject()   || !info[1]->IsNumber()   || !info[2]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    PImage* otherPImage = node::ObjectWrap::Unwrap<PImage>(info[0]->ToObject());
    int x = info[1]->NumberValue();
    int y = info[2]->NumberValue();

    pImage->magickImg_.composite(otherPImage->magickImg_, x, y, Magick::OverCompositeOp);
}

/**
 * Blur image
 * @param radius Radius
 * @param sigma Sigma
 */
NAN_METHOD(PImage::blur)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined()) ||
        (!info[0]->IsNumber() || !info[1]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    double radius = info[0]->NumberValue();
    double sigma = info[1]->NumberValue();

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.blur(radius, sigma);
}

/**
 * Add shadow to image
 * @param xOffset Offset x
 * @param yOffset Offset y
 * @param opacity Opacity 0 ~ 100
 * @param sigma sigma
 */
NAN_METHOD(PImage::shadow)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined() || info[2]->IsUndefined() || info[3]->IsUndefined()) ||
        (!info[0]->IsNumber()   || !info[1]->IsNumber()   || !info[2]->IsNumber()   || !info[3]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    double xOffset = info[0]->NumberValue();
    double yOffset = info[1]->NumberValue();
    double opacity = info[2]->NumberValue();
    double sigma = info[3]->NumberValue();

    Magick::Color shadowColor;

    if (info[4]->IsUndefined() || !info[4]->IsString())
        shadowColor = Magick::Color(0, 0, 0);
    else
    {
        v8::String::Utf8Value hexColor (info[4]->ToString());
        shadowColor = Magick::Color(std::string(*hexColor));
    }

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    Magick::Image imgShadow = pImage->magickImg_;
    imgShadow.backgroundColor(shadowColor);
    imgShadow.shadow(opacity, sigma, xOffset, yOffset);
    imgShadow.composite(pImage->magickImg_, 0, 0, Magick::OverCompositeOp);
    pImage->magickImg_ = imgShadow;
}

NAN_METHOD(PImage::fillImage)
{
    if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsNumber())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    double area = info[0]->NumberValue();
    double red = info[1]->NumberValue();
    double green = info[2]->NumberValue();
    double blue = info[3]->NumberValue();

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    Magick::Color target = Magick::Color("rgb(65535, 65535, 65535)");
    Magick::Color fill   = Magick::ColorRGB(red, green, blue);
    const double fuzz = area * 65535 / 100;

    pImage->magickImg_.colorFuzz(fuzz);
    pImage->magickImg_.opaque(target, fill);
}

NAN_METHOD(PImage::outlineImage)
{
    /*
    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Color target = Magick::Color("rgb(65535, 65535, 65535)");
    Magick::Color fill   = Magick::ColorRGB(1, 0, 0);
    const double fuzz = 80 * 65535 / 100;


    Magick::Image img0 = pImage->magickImg_;
    img0.channel(Magick::MatteChannel);
    img0.threshold(0);

    Magick::Image img1 = img0;
    img1.blur(20, 0);
    img1.threshold(0);

    Magick::Image img2 = img1;
    img2.colorFuzz(fuzz);
    img2.opaque(target, fill);

    img1.composite(img0, 0, 0, Magick::OverCompositeOp);
    img2.composite(img1, 0, 0, Magick::OverCompositeOp);

    //pImage->magickImg_.composite(img2, 0, 0, Magick::CopyBlueCompositeOp);
    img2.composite(pImage->magickImg_, 0, 0, Magick::CopyBlueCompositeOp);
    */

    /*
    Magick::Image imgOutline = pImage->magickImg_;
    imgOutline.colorFuzz(fuzz);
    imgOutline.opaque(target, fill);
    imgOutline.blur(5, 65000);
    imgOutline.threshold(0);
    imgOutline.negate(false);
    imgOutline.resize(Magick::Geometry(imgOutline.columns() + 10, imgOutline.rows() + 10));

    //pImage->magickImg_.composite(imgOutline, 0, 0, Magick::OverCompositeOp);
    imgOutline.composite(pImage->magickImg_, 0, 0, Magick::OverCompositeOp);
    */
}

NAN_METHOD(PImage::setBackgroundColor)
{
    if (info.Length() == 1)
    {
        if (!info[0]->IsString())
        {
            Nan::ThrowError("Invalid argument!");
        }

        v8::String::Utf8Value hexColor (info[0]->ToString());
        PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
        pImage->magickImg_.backgroundColor(Magick::Color(std::string(*hexColor)));
    }
    else if (info.Length() == 3)
    {
        if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber())
        {
            Nan::ThrowError("Invalid argument!");
        }

        double red = info[0]->NumberValue();
        double green = info[1]->NumberValue();
        double blue = info[2]->NumberValue();

        v8::String::Utf8Value hexColor (info[4]->ToString());
        PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
        pImage->magickImg_.backgroundColor(Magick::ColorRGB(red, green, blue));
    }
    else
    {
        Nan::ThrowError("Invalid argument!");
    }
}

NAN_METHOD(PImage::setFilterType)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value filterTypesName (info[0]->ToString());
    Magick::FilterTypes targetFilterType = magickFilterTypes.parse(std::string(*filterTypesName));

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.filterType(targetFilterType);
}

NAN_METHOD(PImage::setType)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value type (info[0]->ToString());

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.magick(std::string(*type));
}

NAN_METHOD(PImage::clone)
{
    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Blob outBlob;
    pImage->magickImg_.write(&outBlob);
    v8::Local<v8::Object> outBuffer = Nan::CopyBuffer((char*) outBlob.data(), outBlob.length()+1).ToLocalChecked();

    v8::Local<v8::Value> argv[1] = { outBuffer };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(1, argv));
}

/**
 * Image to nodejs buffer
 * @return Nodejs buffer
 */
NAN_METHOD(PImage::toBuffer)
{
    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Blob outBlob;
    pImage->magickImg_.write(&outBlob);

    v8::Local<v8::Object> outBuffer = Nan::CopyBuffer((char*) outBlob.data(), outBlob.length()+1).ToLocalChecked();
    info.GetReturnValue().Set(outBuffer);
}

NAN_METHOD(PImage::toFile)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value fileName (info[0]->ToString());

    PImage* pImage = node::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->magickImg_.write(std::string(*fileName));
}
