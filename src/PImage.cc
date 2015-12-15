#include "PImage.h"

Nan::Persistent<v8::Function> PImage::constructor;

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

PImage::PImage(int width, int height)
{
    mImage = new Magick::Image (Magick::Geometry(width, height), Magick::Color());
    mImage->matte(true);
}

PImage::PImage(int width, int height, int red, int green, int blue)
{
    mImage = new Magick::Image (Magick::Geometry(width, height), Magick::ColorRGB(red, green, blue));
}

PImage::PImage(int width, int height, std::string hexBackgroundColor)
{
    mImage = new Magick::Image (Magick::Geometry(width, height), Magick::Color(hexBackgroundColor));
}

PImage::PImage(char* imgBuffer, size_t bufSize)
{
    Magick::Blob *blob = new Magick::Blob(imgBuffer, bufSize);
    mImage = new Magick::Image (*blob);
}
/*
PImage::PImage(const PImage& otherPImage)
{
    mImage = otherPImage.mImage;
}
*/
PImage::~PImage()
{
    delete mImage;
}

void PImage::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;

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

    Nan::SetPrototypeMethod(tpl, "setStrokeColor", setStrokeColor);
    Nan::SetPrototypeMethod(tpl, "setStrokeWidth", setStrokeWidth);
    Nan::SetPrototypeMethod(tpl, "setFillColor", setFillColor);
    Nan::SetPrototypeMethod(tpl, "setFont", setFont);
    Nan::SetPrototypeMethod(tpl, "setFontSize", setFontSize);
    Nan::SetPrototypeMethod(tpl, "getFontMetrics", getFontMetrics);
    Nan::SetPrototypeMethod(tpl, "drawText", drawText);

    Nan::SetPrototypeMethod(tpl, "outlineImage", outlineImage);
    Nan::SetPrototypeMethod(tpl, "setBackgroundColor", setBackgroundColor);
    Nan::SetPrototypeMethod(tpl, "setFilterType", setFilterType);
    Nan::SetPrototypeMethod(tpl, "setType", setType);
    Nan::SetPrototypeMethod(tpl, "clone", clone);
    Nan::SetPrototypeMethod(tpl, "toBuffer", toBuffer);
    Nan::SetPrototypeMethod(tpl, "toFile", toFile);

    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("PImage").ToLocalChecked(), tpl->GetFunction());
}

void PImage::New (const Nan::FunctionCallbackInfo<v8::Value>& info)
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
void PImage::resize (const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined()) ||
	(!info[0]->IsNumber() || !info[1]->IsNumber()))
    {
	Nan::ThrowError("Invalid argument!");
	info.GetReturnValue().SetUndefined();
    }

    int width = info[0]->NumberValue();
    int height = info[1]->NumberValue();

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->resize(Magick::Geometry(width, height));
}

/**
 * Crop image
 * @param x Position x
 * @param y Position y
 * @param width Width
 * @param height Height
 */
void PImage::crop(const Nan::FunctionCallbackInfo<v8::Value>& info)
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

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->chop(Magick::Geometry(x, y));
    pImage->mImage->crop(Magick::Geometry(width, height));
}

/**
 * Composite two image
 * @param imgOverlay The second image
 * @param x Position x
 * @param y Position y
 */
void PImage::composite(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined() || info[2]->IsUndefined()) ||
        (!info[0]->IsObject()   || !info[1]->IsNumber()   || !info[2]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    PImage* otherPImage = Nan::ObjectWrap::Unwrap<PImage>(info[0]->ToObject());
    int x = info[1]->NumberValue();
    int y = info[2]->NumberValue();

    pImage->mImage->composite(*otherPImage->mImage, x, y, Magick::OverCompositeOp);
}

/**
 * Blur image
 * @param radius Radius
 * @param sigma Sigma
 */
void PImage::blur(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined() || info[1]->IsUndefined()) ||
        (!info[0]->IsNumber() || !info[1]->IsNumber()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    double radius = info[0]->NumberValue();
    double sigma = info[1]->NumberValue();

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->blur(radius, sigma);
}

/**
 * Add shadow to image
 * @param xOffset Offset x
 * @param yOffset Offset y
 * @param opacity Opacity 0 ~ 100
 * @param sigma sigma
 */
void PImage::shadow(const Nan::FunctionCallbackInfo<v8::Value>& info)
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
    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    Magick::Image *imgShadow = new Magick::Image(*pImage->mImage);
    imgShadow->backgroundColor(shadowColor);
    imgShadow->shadow(opacity, sigma, xOffset, yOffset);
    imgShadow->composite(*pImage->mImage, 0, 0, Magick::OverCompositeOp);

    delete pImage->mImage;
    pImage->mImage = imgShadow;
}

/**
 * Fill image
 * @param Area area size
 * @param red Red
 * @param green Green
 * @param blue Blue
 */
void PImage::fillImage(const Nan::FunctionCallbackInfo<v8::Value>& info)
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

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    Magick::Color target = Magick::Color("rgb(65535, 65535, 65535)");
    Magick::Color fill   = Magick::ColorRGB(red, green, blue);
    const double fuzz = area * 65535 / 100;

    pImage->mImage->colorFuzz(fuzz);
    pImage->mImage->opaque(target, fill);
}

// Drawing ---------------------------------------------------------------------/

/**
 * Set stroke color
 * @param Color hex color or color name
 */
void PImage::setStrokeColor(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsString())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value rawStrColor (info[0]->ToString());

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->strokeColor(std::string(*rawStrColor));
}

/**
 * Set stroke width
 * @param width Stroke width
 */
void PImage::setStrokeWidth(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsNumber())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    double width = info[0]->NumberValue();

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->strokeWidth(width);
}

/**
 * Set fill color
 * @param Color hex color or color name
 */
void PImage::setFillColor(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsString())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value rawStrColor (info[0]->ToString());

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->fillColor(std::string(*rawStrColor));
}

/**
 * Set font
 * @param fontName Set font from file path (with prefix "@") or system font name
 */
void PImage::setFont (const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsString())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value rawStrFontName (info[0]->ToString());

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->font(std::string(*rawStrFontName));
}

/**
 * Set font size
 * @param fontSize Font size
 */
void PImage::setFontSize (const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsNumber())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    size_t fontSize = info[0]->NumberValue();

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->fontPointsize(fontSize);
}

/**
 * Get font metrics
 * @param text Text
 * @return Object with ascent, descent, textWidth, textHeight and maxHorizontalAdvance
 */
void PImage::getFontMetrics (const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsString())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value rawStrText (info[0]->ToString());
    std::string strText = std::string(*rawStrText);
    Magick::TypeMetric *typeMetric = new Magick::TypeMetric();

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->fontTypeMetrics(strText, typeMetric);

    v8::Local<v8::Object> returnObj = Nan::New<v8::Object>();
    Nan::Set(returnObj, Nan::New<v8::String>("ascent").ToLocalChecked(), Nan::New<v8::Number>(typeMetric->ascent()));
    Nan::Set(returnObj, Nan::New<v8::String>("descent").ToLocalChecked(), Nan::New<v8::Number>(typeMetric->descent()));
    Nan::Set(returnObj, Nan::New<v8::String>("textWidth").ToLocalChecked(), Nan::New<v8::Number>(typeMetric->textWidth()));
    Nan::Set(returnObj, Nan::New<v8::String>("textHeight").ToLocalChecked(), Nan::New<v8::Number>(typeMetric->textHeight()));
    Nan::Set(returnObj, Nan::New<v8::String>("maxHorizontalAdvance").ToLocalChecked(), Nan::New<v8::Number>(typeMetric->maxHorizontalAdvance()));

    info.GetReturnValue().Set(returnObj);
}

/**
 * Draw text to image
 * @param x Top left X
 * @param y Top left Y
 * @param text Text
 */
void PImage::drawText(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsString())
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    int x = info[0]->NumberValue();
    int y = info[1]->NumberValue();
    v8::String::Utf8Value utfText (info[2]->ToString());
    std::string text = std::string(*utfText);

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->draw(Magick::DrawableText(x, y, text));
}

/**
 * Outline image
 * Work in progress
 */
void PImage::outlineImage(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    /*
    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Color target = Magick::Color("rgb(65535, 65535, 65535)");
    Magick::Color fill   = Magick::ColorRGB(1, 0, 0);
    const double fuzz = 80 * 65535 / 100;


    Magick::Image img0 = pImage->mImage;
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

    //pImage->mImage.composite(img2, 0, 0, Magick::CopyBlueCompositeOp);
    img2.composite(pImage->mImage, 0, 0, Magick::CopyBlueCompositeOp);
    */

    /*
    Magick::Image imgOutline = pImage->mImage;
    imgOutline.colorFuzz(fuzz);
    imgOutline.opaque(target, fill);
    imgOutline.blur(5, 65000);
    imgOutline.threshold(0);
    imgOutline.negate(false);
    imgOutline.resize(Magick::Geometry(imgOutline.columns() + 10, imgOutline.rows() + 10));

    //pImage->mImage.composite(imgOutline, 0, 0, Magick::OverCompositeOp);
    imgOutline.composite(pImage->mImage, 0, 0, Magick::OverCompositeOp);
    */
}

/**
 * Set background color
 */
void PImage::setBackgroundColor(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() == 1)
    {
        if (!info[0]->IsString())
        {
            Nan::ThrowError("Invalid argument!");
        }

        v8::String::Utf8Value hexColor (info[0]->ToString());
        PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
        pImage->mImage->backgroundColor(Magick::Color(std::string(*hexColor)));
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
        PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
        pImage->mImage->backgroundColor(Magick::ColorRGB(red, green, blue));
    }
    else
    {
        Nan::ThrowError("Invalid argument!");
    }
}

/**
 * Set filer type
 * @param filerType The filer type name
 */
void PImage::setFilterType(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value filterTypesName (info[0]->ToString());
    Magick::FilterTypes targetFilterType = magickFilterTypes.parse(std::string(*filterTypesName));

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->filterType(targetFilterType);
}

/**
 * Set image type
 * @param type The image type
 */
void PImage::setType(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value type (info[0]->ToString());

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->magick(std::string(*type));
}

/**
 * Clone image object
 * @return Clone image object
 */
void PImage::clone(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Blob outBlob;
    pImage->mImage->write(&outBlob);
    v8::Local<v8::Object> outBuffer = Nan::CopyBuffer((char*) outBlob.data(), outBlob.length()+1).ToLocalChecked();

    v8::Local<v8::Value> argv[1] = { outBuffer };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(1, argv));
}

/**
 * Image to nodejs buffer
 * @return Nodejs buffer
 */
void PImage::toBuffer(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());

    Magick::Blob outBlob;
    pImage->mImage->write(&outBlob);

    v8::Local<v8::Object> outBuffer = Nan::CopyBuffer((char*) outBlob.data(), outBlob.length()+1).ToLocalChecked();
    info.GetReturnValue().Set(outBuffer);
}

/**
 * Image to file
 * @param fileName File name with path
 */
void PImage::toFile(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if ((info[0]->IsUndefined()) ||
        (!info[0]->IsString()))
    {
        Nan::ThrowError("Invalid argument!");
        info.GetReturnValue().SetUndefined();
    }

    v8::String::Utf8Value fileName (info[0]->ToString());

    PImage* pImage = Nan::ObjectWrap::Unwrap<PImage>(info.This());
    pImage->mImage->write(std::string(*fileName));
}
