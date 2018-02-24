//this line will export all the shit
module.exports = {

    autoTest: function(){
        console.log("aa");
    }
    ,
    sampleText : "lol this is sample"

};

function printText(input)
{
    console.log("input is ",input);
}

function test()
{
    console.log("Test function");
}

//or just export the function u need 
module.exports.printText = printText;