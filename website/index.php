<html lang="en">
<head>
    <title>Fingerprint Attendance</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossOrigin="anonymous">
    <link href="styles/sidebar.css" rel="stylesheet">
    <link href="styles/style.css" rel="stylesheet">
</head>

<body>
    <div class="sidebar">
        <div class="border-right" id="sidebar-wrapper">
            <div class="sidebar-heading">
                <h2><b>Fingerprint Attendance System</b></h2>
            </div>
            <div class="sidebar-content">
                <a href="#fingerprints"><b>Browse fingerprints</b></a>
            </div>
            <div class="sidebar-footer">
                &copy; Patrik Bartoš, 2019
            </div>
        </div>
    </div>
    <div class="content">
        <div class="container-fluid" id="fingerprints">
            <br>
            <h1>Saved fingerprints</h1>
            <table style="width:100%">
                <tr>
                    <th style="width:15%">Name</th>
                    <th style="width:15%">Surname</th>
                    <th style="width:auto">Fingerprint</th>
                    <th style="width:20%">Action</th>
                </tr>
                <?php
                    include 'dbconn.php';
                    $conn = OpenCon();
                    $sql = "SELECT id, jmeno, prijmeni, otisk FROM fingerprints";
                    $result = $conn->query($sql);
                    if ($result->num_rows > 0) {
                        while($row = $result->fetch_assoc()){
                            echo "<tr><td>" . $row["jmeno"]. "</td><td>" . $row["prijmeni"] . 
                            "</td><td id='buttons'><button type='button' class='btn btn-secondary' onclick='spoiler(".$row["id"].")'>Show/Hide</button>
                            <p id='".$row["id"]."' style='display:none'>". $row["otisk"]."</p></td>
                            <td id='buttons'><button type='button' class='btn btn-primary' onclick='edit(".$row["id"].")'>Edit</button><button type='button' class='btn btn-danger' onclick='remove(".$row["id"].")'>Remove</button></td></tr>";
                        }
                    }
                ?>
            </table>
        </div>
        <div class="container-fluid" id="editForm">
            <br>
            <h1>Edit row</h1>
            <?php
            $id = $_GET['id'];
            $sql = "SELECT jmeno, prijmeni FROM fingerprints WHERE id = ".$id;
            $result = $conn->query($sql);
            if($result->num_rows > 0){
                while($row = $result->fetch_assoc()) {
                    $name = $row['jmeno'];
                    $surname = $row['prijmeni'];
                }
            }
            ?>
            <form action="update.php" method="POST" role="form">
                <div class="form-group">
                    <input type="hidden" name="id" value="<?=$id;?>">
                    <label for="name">Name</label>
                    <input type="text" class="form-control" name="name" value="<?=$name;?>">
                    <label for="surname">Surname</label>
                    <input type="text" class="form-control" name="surname" value="<?=$surname;?>">
                </div>
                <button style='width:100px' type="submit" class="btn btn-primary">Submit</button>
            </form>
        </div>
    </div>
    <script src="https://code.jquery.com/jquery-3.2.1.min.js">
    </script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous">
    </script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous">
    </script>
    <script>
        $(document).ready(function (){
            if(location.href.indexOf('id=') > -1){
                $("#editForm").show();
            }
            else{
                $("#editForm").hide();
            }
        });

        function spoiler(id){
            var x = document.getElementById(id);
            if (x.style.display === "none") {
                x.style.display = "block";
            }
            else {
                x.style.display = "none";
            }
        }

        function remove(id){
            $.ajax({
                type: "GET",
                url: "remove.php",
                data: { 'id': id },
                success : function() { 
                    location.reload();
                }
            });
        }

        function edit(id){
            var string = 'index.php?id=';
            string += id;
            location.href = string;
            $("#editForm").show();
        }
    </script>
</body>

</html>