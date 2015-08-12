var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: fill_image");

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_img_star.png");

var imageRaw = new Image(imageData);
imageRaw.fillImage(80, 0, 0, 0);

imageRaw.toFile(putil.getCurrentFolder () + "/demo_img_output_fill_image.png");

console.log("[ImagePlus] Completed! \"demo_img_output_fill_image.png\"");
