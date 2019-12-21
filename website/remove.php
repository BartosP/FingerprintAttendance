<?php
    include 'dbconn.php';
    $conn = OpenCon();
    $id = $_GET['id'];
    $sql = "DELETE FROM fingerprints WHERE id=".$id;
    if ($conn->query($sql) === TRUE) {
        echo "Record deleted successfully";
    }
    else{
        echo "Error deleting record: " . $conn->error;
    }
    CloseCon($conn);
?>