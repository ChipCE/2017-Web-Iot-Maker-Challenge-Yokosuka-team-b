var connect = require('connect');
var http = require('http');

var app = connect();

function profile(request,response)
{
    console.log("profile was called");
}

function forum(request,response)
{
    console.log("forum was called");
}


app.use('/profile',profile);
app.use('/forum',forum);

http.createServer(app).listen(8888);
console.log("server is running...");

