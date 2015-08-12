var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: clone");

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_img_star.png");

var imageBackground = new Image(200, 300, "#FFFFFF");
var imageStar = new Image(imageData);
imageStar.blur(10, 50);
var imageStar2 = imageStar.clone();
imageBackground.composite(imageStar, 0, 0);
imageStar2.fillImage(80, 0, 0, 0);
imageBackground.composite(imageStar2, 50, 0);
imageBackground.composite(imageStar, 30, 50);

imageBackground.toFile(putil.getCurrentFolder () + "/demo_img_output_clone.png");

console.log("[ImagePlus] Completed! \"demo_img_output_clone.png\"");
