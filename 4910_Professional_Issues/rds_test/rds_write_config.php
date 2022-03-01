<!DOCTYPE html>
<html>
    <head>
        <title>EC2 Web Server And RDS Connection</title>
        <meta http-equiv="refresh" content="10,URL=/index.php" />
    </head>
    <body>
        <div class="container">
            <?php
                $ep = $_POST["endpoint"] . ":3306";
                $db = $_POST["database"];
                $un = $_POST["username"];
                $pw = $_POST["password"];

                $conn = new mysqli($ep, $un, $pw, $db);

                if ($conn -> connect_error) {
                    die("Connection failed: " . $conn -> connect_error);
                    exit();
                } else {
                    $file = fopen("rds_info.php", "w");
                    $data = "<?php \$RDS_url='" . $ep . "'; \$RDS_db='" . $db . "'; \$RDS_user='" . $un . "'; \$RDS_pwd='" . $pw . "'; ?>";
                    fwrite($file, $data);
                    fclose($file);
                }

                Print "pulling data, redirecting shortly...";

                $conn -> close();
            ?>
        </div>
    </body>
</html>