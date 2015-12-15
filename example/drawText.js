var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var fs = require("fs");

console.log("[ImagePlus] Start example: drawText");

var text = "Hello World";
var text2 = "after 10px";

var imageData = fs.readFileSync(putil.getCurrentFolder () + "/../resources/demo_img_star.png");

var imageRaw = new Image(imageData);
imageRaw.setStrokeColor("blue");
imageRaw.setStrokeWidth(0.5);
imageRaw.setFillColor("#00EEFF");
imageRaw.setFont("helvetica-bold");
imageRaw.setFontSize(15);

var fontMetric = imageRaw.getFontMetrics(text);

console.log("Text width: " + fontMetric.textWidth + "px");

imageRaw.drawText(5, 60, text);
imageRaw.setFontSize(10);
imageRaw.drawText(5 + fontMetric.textWidth + 10, 60, text2);

imageRaw.toFile(putil.getCurrentFolder () + "/demo_img_output_drawText.png");

console.log("[ImagePlus] Completed! \"demo_img_output_fill_image.png\"");
