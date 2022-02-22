<!DOCTYPE html>
<html>
  <head>
    <title>EC2 Web Server And RDS Connection</title>
  </head>
  
  <body>
    <div class="container">
      <?php
        include("rds_info.php");

        if ($RDS_url == "") {
          include("rds_config.php");
        } else {
          include("rds_read.php");
        }

        $conn -> close();
      ?>
    </div>
  </body>
</html>
