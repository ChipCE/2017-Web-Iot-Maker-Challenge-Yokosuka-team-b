var facModule = require("./factoryModule");

var fac2 = facModule();
fac2.str = "overwriten by fac2";
console.log(fac2.str);