<?php
    $servername = "localhost";
    $username = "teamb";
    $password = "password";
    $dbname = "TrainDB";

    //data from db 
    $people;
    $temperature;
    $humidity;
    $point;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) 
    {
        die("Connection failed: " . $conn->connect_error);
    } 

    $sql = "SELECT * FROM TrainLog WHERE id = (SELECT max(id) from TrainLog)";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) 
    {
        // output data of each row
        if($row = $result->fetch_assoc()) 
        {
            $people=$row["people"];
            $temperature=$row["temperature"];
            $humidity=$row["humidity"];
            $point = 0.81 * $temperature + 0.01 * $humidity * (0.99 * $temperature - 14.3) + 46.3;
        }
    } 
    $conn->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    
    <!-- If IE use the latest rendering engine -->
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <!-- Set the page to the width of the device and set the zoon level -->
    <meta name="viewport" content="width = device-width, initial-scale = 1">

    <title>Team B</title>

    <!--  CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css">
    <link rel="stylesheet" href="style.css">

    <!-- JavaScript -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js" defer></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" defer></script>
    <script src="script.js" defer></script>
</head>
<body>
    <!-- nav bar -->
    <nav class="navbar navbar-inverse" id="topNavBar">
        <div class="container-fluid">
            <!-- logo -->
            <div class="navbar-header">
                <a href="index.html" class="navbar-brand">Team B@横須賀</a>
                <p class="navbar-text">
                        車両混雑状況お知らせシステム
                </p>
            </div>
            <!-- right menu -->
            <ul class="nav navbar-nav navbar-right">
                <li><a href="https://github.com/ChipTechno/2017-Web-Iot-Maker-Challenge-Yokosuka-team-b">About</a></li>
            </ul>
        </div>
    </nav>


    <!-- main content.............................................-->
    <div class="container-fluid">
        <div class="row">
            <div class="col-md-12 col-sm-12 col-xs-12">
                <div class="well  text-center">
 逸見駅 <br/>
次の電車:普通品川行き　xx時xx分<br/>
<div id="cTime"></div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-md-3 col-sm-4 col-xs-12">
                <div class="well cab-info">
                    <h4 class="text-center text-primary">1号車</h4>
                    乗客数:
                    <h2 class="text-right">No data /52</h2>
                    <p>温度:<code>No data</code> &nbsp; &nbsp;  湿度:<code>No Data</code></p>
                    <p>不快指数:<code>No data</code></p>
                </div>
            </div>
            <div class="col-md-3 col-sm-4 col-xs-12">
                <div class="well cab-info" id="trainStat">
                    

                </div>
            </div>
            <div class="col-md-3 col-sm-4 col-xs-12">
                <div class="well cab-info">
                    <h4 class="text-center text-primary">3号車(弱冷房車)</h4>
                    乗客数:
                    <h2 class="text-right">No data /52</h2>
                    <p>温度:<code>No data</code> &nbsp; &nbsp;  湿度:<code>No Data</code></p>
                    <p>不快指数:<code>No data</code></p>
                </div>
            </div>
            <div class="col-md-3 col-sm-4 col-xs-12">
                <div class="well cab-info">
                    <h4 class="text-center text-primary">4号車</h4>
                    乗客数:
                    <h2 class="text-right">No data /52</h2>
                    <p>温度:<code>No data</code> &nbsp; &nbsp;  湿度:<code>No Data</code></p>
                    <p>不快指数:<code>No data</code></p>
                </div>
            </div>
        </div>
    </div>
    <!-- end main content.............................................-->
            
</body>
</html>
