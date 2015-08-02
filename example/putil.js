var path = require("path");

module.exports = {
    getCurrentFolder: function () {
        return __filename.substring(0, __filename.lastIndexOf(path.sep));
    }
}