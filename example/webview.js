var putil = require("./putil");
var imageplus = require("../index");
var Image = imageplus.Image;

var http = require('http');
var fs = require('fs');

console.log("[ImagePlus] Start example: webview");
console.log("[ImagePlus] You can connect the web server on your bowser: http://localhost:3000");

http.createServer(function (req, res) {
    
    fs.readFile(putil.getCurrentFolder () + "/../resources/imageplus_logo.png", function (err, data) {
        if (err)
        {
            res.end("ERROR! " + err.message);
            return;
        }
        var img = new Image(data);
        img.resize(100, 150);
        res.writeHead(200, {'Content-Type': 'image/png'});
        res.end(img.toBuffer());
    });
    
}).listen(3000);