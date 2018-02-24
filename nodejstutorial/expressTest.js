const express = require('express');
const app = express();

app.get('/',(reg,res)=>{
    debugger;
    res.send("here you are");

});

app.listen(3000,()=> console.log("server is up on 3000"));