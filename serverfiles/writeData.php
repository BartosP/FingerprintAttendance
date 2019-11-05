<?php
    $name = $_GET["name"];
    $surname = $_GET["surname"];
    $fileContent = ";".$name.";".$surname.";";
    file_put_contents("data.txt", $fileContent);
    header("Location: index.html");
    die();
?>