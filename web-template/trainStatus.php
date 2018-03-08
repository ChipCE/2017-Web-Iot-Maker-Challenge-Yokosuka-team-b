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

echo '<h4 class="text-center text-primary">2号車</h4>';
echo '乗客数:';
echo '<h2 class="text-right">'.$people.'/52</h2>';
echo '<p>温度:<code>'.$temperature.'</code> &nbsp; &nbsp;  湿度:<code>'.$humidity.'</code></p>';
echo '<p>不快指数:<code>'.$point.'</code></p>';
?>