var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: composite");

var imageBackgroundData = fs.readFileSync(putil.getCurrentFolder () + "/resources/demo_img.png");
var imageBackground = new Image(imageBackgroundData);

var imageStarData = fs.readFileSync(putil.getCurrentFolder () + "/resources/demo_img_star.png");
var imageStar = new Image(imageStarData);
imageStar.resize(90, 90);
imageBackground.composite(imageStar, 120, 140);
imageBackground.toFile(putil.getCurrentFolder () + "/demo_img_output_composite.png");

console.log("[ImagePlus] Completed! \"demo_img_composite.png\"");