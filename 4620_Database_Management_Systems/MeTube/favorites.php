<!-- Original file: MeTube-master/favorites.php -->

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

        <title>Favorites List</title>
    </head>

    <body>
        <?php
            $fav_ids = array();
            $user_id = get_current_user_id();
            $fav_result = run_query("SELECT * FROM favorites WHERE user_id = '$user_id'");
            while ($entry = $fav_result -> fetch_assoc())
            {
                array_push($fav_ids, $entry["media_id"]);
            }

            if (count($fav_ids) > 0)
            {
                $query = "SELECT * FROM media WHERE id IN ('" . $fav_ids[0] . "'";
                    
                for ($x = 1; $x < count($fav_ids); $x++)
                {
                    $query = $query . ", '" . $fav_ids[$x] . "'";
                }
                $query = $query . ")";

                $media_result = run_query($query);

                if (!$media_result)
                {
                    exit ("Could not query the media table in the database");
                }
        ?>

        <table>
            <caption>My Favorite Media</caption>
            <tr>
                <th>Title</th>
                <th>Uploader</th>
                <th>Date Uploaded</th>
                <th>Media Type</th>
                <th>Category</th>
                <th>Total Views</th>
                <th>Remove?</th>
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
                    echo "<td><a href=remove_from_favorites.php?id=" . $media_id . ">remove</a></td>";
                    echo "<tr>";
                }
            ?>
        </table>

        <?php
            }
            else
            {
                echo "<h2>You have no favorited media!</h2>";
            }
        ?>
    </body>
</html>