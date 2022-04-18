<!DOCTYPE html>

<?php
    include_once "navbar.php";
	include_once "helpers.php";
?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />

        <style type="text/css">

            #view_forms {
                padding: 10px;
            }

            #view_forms form {
                margin: 10px;
                padding: 5px;
                background-color: lightgray;
            }

        </style>
        
        <title>Viewing Media</title>
    </head>

    <body>

    <?php
        if(!isset($_GET["id"]))
        {
            echo "<meta http-equiv='refresh' content='0;url=browse.php'>";
        }
        
        $media_id = $_GET["id"];

        run_query("UPDATE media SET view_count = view_count + 1 WHERE id = " . $media_id);

        $result = run_query("SELECT * FROM media WHERE id = " . $media_id);
        $media_data = $result -> fetch_assoc();

        $file_name = $media_data["file_name"];
        $file_type = $media_data["type"];
        $file_path = $media_data["path"];
        $upload_date = $media_data["upload_date"];
        $title = $media_data["title"];
        $category = $media_data["category"];
        $view_count = $media_data["view_count"];
        $description = $media_data["description"];

        $tags_result = run_query("SELECT tag FROM tags WHERE media_id = " . $media_id);
        $tags_array = $tags_result -> fetch_all(MYSQLI_NUM);

        $uploader_id = $media_data["uploader_id"];
        $uploader_result = run_query("SELECT username FROM users WHERE id = " . $uploader_id);
        $uploader = ($uploader_result -> fetch_row())[0];

        if ($file_type == "image")
        {
            echo "<img src=" . $file_path . "/>";
        }
        else if ($file_type == "audio")
        {
            echo "<audio controls>";
            echo "<source src=" . $file_path . " type='audio/mpeg'>";
            echo "<source src=" . $file_path . " type='audio/ogg'>";
            echo "<source src=" . $file_path . " type='audio/wav'>";
            echo "Your browser does not support the audio element";
            echo "</audio>";
        }
        else
        {
            echo "<video controls>";
            echo "<source src=" . $file_path . " type='video/mp4'>";
            echo "<source src=" . $file_path . " type='video/webm'>";
            echo "<source src=" . $file_path . " type='video/ogg'>";
            echo "Your browser does not support the video element";
            echo "</video>";
        }
    ?>
        <div id="media_info">
            <table>
                <caption>Media Information</caption>
                <tr>
                    <th>Title</th>
                    <th>Uploaded By</th>
                    <th>Date Uploaded</th>
                    <th>Total Views</th>
                    <th>Description</th>
                    <th>Category</th>
                    <th>Tags</th>
                </tr>
                <tr>
                    <?php
                        echo "<td>" . $title . "</td>";
                        echo "<td>" . $uploader . "</td>";
                        echo "<td>" . $upload_date . "</td>";
                        echo "<td>" . $view_count . "</td>";
                        echo "<td>" . $description . "</td>";
                        echo "<td>" . $category . "</td>";
                        echo "<td>";
                        if ($tags_array)
                        {
                            foreach ($tags_array as $tag)
                            {
                                echo $tag[0] . " ";
                            }
                        }
                        echo "</td>";
                    ?>
                </tr>
            </table>
        </div>

        <a href="process_download.php?id=<?php echo $media_id ?>"><button type="button">Download media</button></a>

        <div id="view_forms">
            <form method="post" action="add_to_favorites.php">
                <input type="hidden" name="media_id" value="<?php echo $media_id;?>">
                <input type="submit" value="Add media to favorites">
            </form>

            <form method="post" action="add_to_playlist.php">
                <input type="hidden" name="media_id" value="<?php echo $media_id;?>">
                <div class="form_field">
                    <label>Playlist name:</label>
                    <input type="text" name="playlist_name">
                </div>
                <input type="submit" value="Add media to playlist">
            </form>

            <form method="post" action="add_comment.php">
                <input type="hidden" name="media_id" value="<?php echo $media_id;?>">
                <div class="form_field">
                    <label>Add comment:</label>
                    <input type="text" name="comment" required>
                </div>
                <input type="submit" value="Add comment to media">
            </form>
        </div>

        <table>
            <caption>Comments</caption>
            <tr>
                <th> User </th>
                <th> Comment </th>
            </tr>

            <?php
                $comments_result = run_query("SELECT * FROM comments WHERE media_id = '$media_id'");

                if ($comments_result)
                {
                    while ($row = $comments_result -> fetch_assoc()) {
                        $commenter_id = $row["user_id"];
                        $commenter_result = run_query("SELECT username FROM users WHERE id = '$commenter_id'");
                        $commenter = ($commenter_result -> fetch_row())[0];

                        $text = $row["comment_text"];
                        echo "<tr><td>$commenter</td>";
                        echo "<td>$text</td></tr>";
                    }
                }
            ?>
        </table>
    </body>
</html>
