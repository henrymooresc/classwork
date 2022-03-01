<h3>DB Stuff</h3>
<?php
    include("rds_info.php");
    
    $conn = new mysqli($RDS_url, $RDS_user, $RDS_pwd, $RDS_db);

    if ($conn -> connect_error) {
        die("Connection failed: " . $conn -> connect_error);
        exit();
    }

    $q_data = $conn -> query("SELECT * FROM users");

    Print "<table border cellpadding=3>";
    Print "<tr><th width=100>ID</th> " .
        "<th width=100>First name</th> " .
        "<th width=100>Last name</th> " .
        "<th width=200>Email</th> " .
        "<th width=200>Phone number</th> " .
        "<th width=100>City</th> " .
        "<th width=100>State</th> " .
        "<th width=100>Street</th> " .
        "<th width=100>House number</th> " .
        "<th width=100>Password hash</th> " .
        "<th width=100>User type</th></tr>";
    
    while($row = $q_data -> fetch_array(MYSQLI_NUM)) {
        for ($x = 0; $x <= 10; $x++) {
            if (! isset($row[$x])) {
                $row[$x] = null;
            }
        }
        Print "<tr><td>" . $row[0] . "</td> ";
        Print "<td>" . $row[1] . "</td> ";
        Print "<td>" . $row[2] . "</td> ";
        Print "<td>" . $row[3] . "</td> ";
        Print "<td>" . $row[4] . "</td> ";
        Print "<td>" . $row[5] . "</td> ";
        Print "<td>" . $row[6] . "</td> ";
        Print "<td>" . $row[7] . "</td> ";
        Print "<td>" . $row[8] . "</td> ";
        Print "<td>" . $row[9] . "</td> ";
        Print "<td>" . $row[10] . "</td></tr>";
    }
    Print "</table>";

    $conn -> close();

    if (isset($_POST["end_button"])) {
        unset($_POST["end_button"]);
        $file = fopen("rds_info.php", "w");
        $data = "<?php \$RDS_url=''; \$RDS_db=''; \$RDS_user=''; \$RDS_pwd=''; ?>";
        fwrite($file, $data);
        fclose($file);
        header("Refresh:2");
    }
?>

<form method="post">
    <input type="submit" name="end_button" value="End Connection"/>
</form>