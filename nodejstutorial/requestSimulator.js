function getRequest(requestNumber)
{
    console.log("taking order No.",requestNumber);

    //call the db querry
    databaseQuery(function(){
        console.log("Yay! Got the data");
    })
}

function databaseQuery(callback)
{
    //query takes 5 secs
    setTimeout(callback,5000);
}


for(i=0;i<10;i++)
    getRequest(i);