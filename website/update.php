<?php
    include 'dbconn.php';
    $conn = OpenCon();
    $id = $_POST['id'];
    $name = $_POST['name'];
    $surname = $_POST['surname'];
    $sql="UPDATE fingerprints SET jmeno = '$name', prijmeni = '$surname' WHERE ID=".$id;
    $conn->query($sql);
    header("Location: /");
?>