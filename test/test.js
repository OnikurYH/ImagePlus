var imageplus = require("../index");
var Image = imageplus.Image;

var assert = require("assert");
var fs = require("fs");

var IMAGE_INPUT = "test/demo_img.png";
var IMAGE_OUTPUT = "test/demo_img_output";

describe('Image', function() {
    describe('.resize()', function () {
        it('should resize successfully', function () {
            var img = new Image(fs.readFileSync(IMAGE_INPUT));
            img.resize(100, 100);
        });
    });
    
    describe('.crop()', function () {
        it('should crop successfully', function () {
            var img = new Image(fs.readFileSync(IMAGE_INPUT));
            img.crop(10, 10, 30, 30);
        });
    });
    
    describe('.toFile()', function () {
        it('should save to file successfully', function () {
            var img = new Image(fs.readFileSync(IMAGE_INPUT));
            img.toFile(IMAGE_OUTPUT + ".png");
            fs.unlinkSync(IMAGE_OUTPUT + ".png");
        });
    });
});