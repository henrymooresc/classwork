<!DOCTYPE html>
<html>
  <head>
    <title>EC2 Web Server And RDS Connection</title>
  </head>
  
  <body>
    <h2>Home Page</h2>
    <div class="container">
      <?php
        include("rds_info.php");

        if ($RDS_url == "") {
          include("rds_config.php");
        } else {
          include("rds_read.php");
        }

      ?>
    </div>
  </body>
</html>
