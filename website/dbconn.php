<?php
    function OpenCon(){
        $dbhost = "10.0.0.41";
        $dbuser = "User";
        $dbpass = "Password";
        $db = "attendance";
        $conn = new mysqli($dbhost, $dbuser, $dbpass, $db) or die("Connect failed: %s\n". $conn -> error);
        return $conn;
    }
    function CloseCon($conn){
        $conn -> close();
    }
?>