var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: outline");

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_img_star.png");

var imageRaw = new Image(imageData);
imageRaw.outlineImage();

imageRaw.toFile(putil.getCurrentFolder () + "/demo_img_output_outline.png");

console.log("[ImagePlus] Completed! \"demo_img_output_outline.png\"");
