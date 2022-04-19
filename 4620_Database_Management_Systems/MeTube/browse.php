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

        <title>Browsing</title>
    </head>

    <body>
        <?php
            if (isset($_GET["category"]))
            {
                $query = "SELECT * FROM media WHERE category = '" . $_GET['category'] . "'";
            }
            else if (isset($_SESSION["search_results"]))
            {
                $search_result_ids = $_SESSION["search_results"];
                $query = "SELECT * FROM media WHERE id IN ('" . $search_result_ids[0] . "'";
                
                for ($x = 1; $x < count($search_result_ids); $x++)
                {
                    $query = $query . ", '" . $search_result_ids[$x] . "'";
                }
                $query = $query . ")";

                session_unset("search_results");
            }
            else 
            {
                $query = "SELECT * from media";
            }

            $media_result = run_query($query);

            if (!$media_result)
            {
                exit ("Could not query the media table in the database");
            }
        ?>

        <table class="media_table">
            <caption>Media Results</caption>
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
                    echo "<td><a href=channel.php?id=" . $uploader_id . ">" . $uploader . "</a></td>";
                    echo "<td>" . $upload_date . "</td>";
                    echo "<td>" . $file_type . "</td>";
                    echo "<td>" . $category . "</td>";
                    echo "<td>" . $view_count . "</td>";
                    echo "<td><a href=view.php?id=" . $media_id . ">view</a></td>";
                    echo "<tr>";
                }
            ?>
        </table>
    </body>
</html>