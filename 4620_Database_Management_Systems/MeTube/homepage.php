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
                $subscriptions_result = run_query("SELECT * FROM subscriptions WHERE sub_id = '$id'");
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
                while ($entry = $media_result -> fetch_assoc())
                {
                    $media_id = $entry["id"];
                    $file_type = $entry["type"];
                    $upload_date = $entry["upload_date"];
                    $title = $entry["title"];
                    $category = $entry["category"];
                    $view_count = $entry["view_count"];
                    $uploader_id = $entry["uploader_id"];

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

        <table>
            <caption>My Subscriptions</caption>
            <tr>
                <th>Channel Owner</th>
                <th>Link to Channel</th>
            </tr>
            <?php
                while ($entry = $subscriptions_result -> fetch_assoc())
                {
                    $sub_to_id = $entry["sub_to_id"];

                    $username_result = run_query("SELECT username FROM users WHERE id = " . $sub_to_id);
                    $channel_owner = ($username_result -> fetch_row())[0];
                
                    echo "<tr>";
                    echo "<td>" . $channel_owner . "</td>";
                    echo "<td><a href=channel.php?id=" . $sub_to_id . ">view</a></td>";
                    echo "<tr>";
                }
            ?>
        </table>

        <?php
            }
        ?>
    </body>
</html>