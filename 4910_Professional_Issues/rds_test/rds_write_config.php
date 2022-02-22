<!DOCTYPE html>
<html>
    <head>
        <title>EC2 Web Server And RDS Connection</title>
    </head>
    <body>
        <div class="container">
            <?php
                $ep = $_POST["endpoint"];
                $ep = str_replace(":3306", "", $ep);
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

                $conn -> close();
            ?>
        </div>
    </body>
</html>