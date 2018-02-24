//file
var fs = require('fs');
fs.writeFileSync("text.txt","sample str");
console.log(fs.readFileSync("text.txt").toString());

console.log(__filename);
console.log(__dirname);


//path
var path = require('path');
var websiteHome = "Desktop/folder//www/index.php";
var websiteAbout = "Desktop/folder/www/about.php";
console.log(websiteHome);
console.log(websiteAbout);
console.log(path.normalize(websiteHome));
console.log(path.normalize(websiteAbout));
console.log(path.basename(websiteHome));
console.log(path.basename(websiteAbout));

setInterval(function(){
    console.log(".");
},1000);

