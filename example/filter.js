var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: filter");

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_img_star_small.png");

var imageBackground = new Image(128, 640, 1, 1, 1);

var imageRaw = new Image(imageData);
imageRaw.resize(128, 128);
imageBackground.composite(imageRaw, 0, 0);

var imagePoint = new Image(imageData);
imagePoint.setFilterType("Point");
imagePoint.resize(128, 128);
imageBackground.composite(imagePoint, 0, 128);

var imageGaussian = new Image(imageData);
imageGaussian.setFilterType("Gaussian");
imageGaussian.resize(128, 128);
imageBackground.composite(imageGaussian, 0, 256);

var imageMitchell = new Image(imageData);
imageMitchell.setFilterType("Mitchell");
imageMitchell.resize(128, 128);
imageBackground.composite(imageMitchell, 0, 384);

var imageBessel = new Image(imageData);
imageBessel.setFilterType("Bessel");
imageBessel.resize(128, 128);
imageBackground.composite(imageBessel, 0, 512);

imageBackground.toFile(putil.getCurrentFolder () + "/demo_img_output_filter.png");

console.log("[ImagePlus] Completed! \"demo_img_output_filter.png\"");