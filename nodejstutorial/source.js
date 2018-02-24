var myObj = {
    name : "objname",
    type : "dummy"
};

function testFunc(a,b)
{
    console.log(a+b);
    return a+b;
}

var varfunc = function()
{
    console.log('var function output');
}

function slowFunc(callback)
{
    setTimeout(callback,1000);
}




console.log(myObj);
console.log(testFunc(3,4));
varfunc();

setTimeout(varfunc,1000);