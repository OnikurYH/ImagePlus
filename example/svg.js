var putil = require("./putil");
var fs = require("fs");
var imageplus = require("../index");
var Image = imageplus.Image;

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_svg_star.svg");
var imageRaw = new Image(imageData);
imageRaw.toFile(putil.getCurrentFolder () + "/demo_svg_star_output_image.png");
