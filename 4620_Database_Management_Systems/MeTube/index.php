<!DOCTYPE html>

<html>
    <body>
        <?php
            include_once "navbar.php";
            echo "<h1> MeTube Project, Spring 2022 </h1>";
            echo "<h4> Made by: Henry Moore </h4>";
            echo "<p>redirecting to homepage...</p>";
            header("Refresh:3; url=homepage.php");
        ?>
    </body>
</html>
