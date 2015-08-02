var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: shadow");

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/resources/demo_img_star.png");

var imageStar = new Image(imageData);
imageStar.shadow(0, 0, 80, 2);
imageStar.toFile(putil.getCurrentFolder () + "/demo_img_output_shadow.png");

console.log("[ImagePlus] Completed \"demo_img_output_shadow.png\"");