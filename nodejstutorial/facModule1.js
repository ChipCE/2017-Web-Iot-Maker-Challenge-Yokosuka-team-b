var facModule = require("./factoryModule");

var fac1 = facModule();
fac1.str = "overwriten by fac1";
console.log(fac1.str);