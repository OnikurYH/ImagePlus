# ImagePlus
![ImagePlus Logo](/resources/imageplus_logo.png)

A node.js imagemagick API

## Install
### Mac OS X
1. Install [ImageMagick](http://www.imagemagick.org/script/index.php) from Homebrew, MacPorts or Binary
2. Install npm-gyp `npm install npm-gyp -g`
3. Run `npm install imageplus`

### Linux
#### Debian / Ubuntu
1. Install [ImageMagick](http://www.imagemagick.org/script/index.php) and Magick++ `sudo apt-get install imagemagick libmagick++-dev`
2. Install npm-gyp `npm install npm-gyp -g`
3. Run `npm install imageplus`

#### Redhat / CentOS / Fedora
1. Install [ImageMagick](http://www.imagemagick.org/script/index.php) and Magick++ `sudo apt-get install ImageMagick ImageMagick-devel`
2. Install npm-gyp `npm install npm-gyp -g`
3. Run `npm install imageplus`

### Windows
Windows version may not stable on this time

## Usage
ImagePlus is a object oriented imagemagick API, you can easy to edit a image / SVG file.

First, you need to require the ImagePlus module on your script.
```javascript
var imageplus = require("imageplus");
var Image = imageplus.Image;
```
and now, you can use the API

## API
### New image
`new Image(int width, int height, double red, double green, double blue)`
```javascript
var img = new Image(128, 640, 1, 1, 1);
```
width and height is pixel unit

red, green, blue is between 0.0 to 1.0

Also, you can use html color

`new Image(int width, int height, string htmlHexColor)`
```javascript
var img = new Image(128, 640, "#FF0000");
```

### Load image to "Image" object
`new Image(buffer imgBuffer)`
```javascript
var myImgBuffer = fs.readFileSync("./image.png");
var img = new Image(myImgBuffer);
```
ImagePlus is allow you to input the buffer object

### Resize
`Image.resize(int width, int height);`
```javascript
img.resize(100, 130);
```

### Composite
`Image.composite(Image otherImage, int x, int y);`
```javascript
var imgBackground = new Image(fs.readFileSync("./background.png"));
var imgOther = new Image(fs.readFileSync("./other.png"));
imgBackground.composite(imgOther, 100, 30);
```

### Shadow
`Image.shadow(double x, double y, double alpha, double offset);`
```javascript
img.shadow(0, 0, 70, 3);
```

### Background color
`Image.setBackgroundColor(double red, double green, double blue)`
```javascript
img.setBackgroundColor(0.7, 0.2, 0.5);
```
`Image.setBackgroundColor(string htmlHexColor)`
```javascript
img.setBackgroundColor("#00AAFF");
```

### Filter
`Image.setFilterType(string filtertype);`

Here is a filter list
- Undefined
- Point
- Box
- Triangle
- Hermite
- Hanning
- Hamming
- Blackman
- Gaussian
- Quadratic
- Cubic
- Catrom
- Mitchell
- Lanczos
- Bessel
- Sinc

```javascript
img.setFilterType("Point");
```

### File type
`Image.setType(string fileType);`
```javascript
img.setType("jpg");
```

### Save to buffer
`Image.toBufer();`
```javascript
var outputBuffer = img.toBuffer();
```

### Save to file
`Image.toFile(string save_to_file_path);`
```javascript
img.toFile("output_file.bmp");
```

## Develop
Develop by: Onikur

## License
GNU GPL v3
