<?php
    class fingerprint{
        public $link='';
        function __construct($otisk, $name, $surname){
            $this->connect();
            $this->storeInDB($otisk, $name, $surname);
        }
        function connect(){
            $this->link = mysqli_connect('localhost', 'root', '') or die('Cannot connect to the DB');
            mysqli_select_db($this->link,'attendance') or die('Cannot select the DB');
        }
        function storeInDB($otisk, $name, $surname){
            $query = "INSERT INTO fingerprints (jmeno, prijmeni, otisk) VALUES ('".$name."', '".$surname."', '".$otisk."')";
            $result = mysqli_query($this->link, $query) or die('Errant query:  '.$query);
        } 
    }
    if($_GET['otisk'] != '' && $_GET['name'] != '' && $_GET['surname'] != ''){
        $fingerprint = new fingerprint($_GET['otisk'], $_GET['name'], $_GET['surname']);
    }
?>