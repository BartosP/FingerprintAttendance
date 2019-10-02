<?php
    class fingerprint{
        public $link='';
        function __construct($otisk){
            $this->connect();
            $this->storeInDB($otisk);
        }
        function connect(){
            $this->link = mysqli_connect('localhost', 'root', '') or die('Cannot connect to the DB');
            mysqli_select_db($this->link,'attendance') or die('Cannot select the DB');
        }
        function storeInDB($otisk){
            $query = "INSERT INTO fingerprints SET otisk='".$otisk."'";
            $result = mysqli_query($this->link, $query) or die('Errant query:  '.$query);
        } 
    }
    if($_GET['otisk'] != ''){
        $fingerprint = new fingerprint($_GET['otisk']);
    }
?>