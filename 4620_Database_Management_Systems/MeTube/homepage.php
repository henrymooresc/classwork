<!-- Original file: MeTube-master/browse.php -->

<!DOCTYPE html>

<?php
	include_once "navbar.php";
	include_once "helpers.php";
?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />
        <style type="text/css">
            
        </style>

        <title>MeTube Homepage</title>
    </head>

    <body>
        <h2>Welcome to MeTube <?php if (isset($_SESSION['username'])) echo $_SESSION['username'];?></h2>

        <div id="categories">
            <h3>Video Categories:</h3>
            <ul>
                <li><a href="browse.php?category=entertainment">Entertainment</a></li>
                <li><a href="browse.php?category=kids">Kids</a></li>
                <li><a href="browse.php?category=educational">Educational</a></li>
                <li><a href="browse.php?category=meme">Meme</a></li>
                <li><a href="browse.php?category=Other">Other</a></li>
            </ul>
        </div>
        
        <?php 
            if (isset($_SESSION["username"]))
            {
                $id = get_current_user_id();
                $media_result = run_query("SELECT * FROM media WHERE uploader_id = '$id' ORDER BY upload_date");
        ?>

        <a href="upload.php"><button type="button">Upload Media</button></a>

        <table>
            <caption>My Stuff</caption>
            <tr>
                <th>Title</th>
                <th>Uploader</th>
                <th>Date Uploaded</th>
                <th>Media Type</th>
                <th>Category</th>
                <th>Total Views</th>
            </tr>
            <?php
                while ($row = $media_result -> fetch_assoc())
                {
                    $media_id = $row["id"];
                    $file_type = $row["type"];
                    $upload_date = $row["upload_date"];
                    $title = $row["title"];
                    $category = $row["category"];
                    $view_count = $row["view_count"];
                    $uploader_id = $row["uploader_id"];

                    $username_result = run_query("SELECT username FROM users WHERE id = " . $uploader_id);
                    $uploader = ($username_result -> fetch_row())[0];
                
                    echo "<tr>";
                    echo "<td>" . $title . "</td>";
                    echo "<td>" . $uploader . "</td>";
                    echo "<td>" . $upload_date . "</td>";
                    echo "<td>" . $file_type . "</td>";
                    echo "<td>" . $category . "</td>";
                    echo "<td>" . $view_count . "</td>";
                    echo "<td><a href=view.php?id=" . $media_id . ">view</a></td>";
                    echo "<tr>";
                }
            ?>
        </table>

        <?php
            }
        ?>
    </body>
</html>