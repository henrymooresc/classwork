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

        <title>Playlist</title>
    </head>

    <body>
        <?php
            $playlist_id = $_GET["id"];
            $playlist_head_result = run_query("SELECT * FROM playlists WHERE id = '$playlist_id'");
            $playlist_head = $playlist_head_result -> fetch_assoc();

            if (is_null($playlist_head["media_id"]))
            {
                echo "<h2>This playlist is empty!</h2>";
            }
            else
            {
        ?>

        <table id="playlist_media_table">
            <caption><?php echo $playlist_head["name"]?> Playlist:</caption>
            <tr>
                <th>Media Index</th>
                <th>Media Name</th>
                <th>Play From Here</th>
                <th>Remove?</th>
            </tr>
            <?php
                $media_id = $playlist_head["media_id"];
                $media_info_result = run_query("SELECT * FROM media WHERE id = '$media_id'");
                $media_info = $media_info_result -> fetch_assoc();

                $name = $media_info["file_name"];
                $index = 1;

                echo "<tr>";
                echo "<td>" . $index . "</td>";
                echo "<td>" . $name . "</td>";
                echo "<td><a href=view.php?playlist_id=" . $playlist_id . ">play</a></td>";
                echo "<td><a href=remove_from_playlist.php?id=" . $playlist_id . ">remove</a></td>";
                echo "<tr>";

                $next_id = $playlist_head["next_id"];

                while (!is_null($next_id))
                {
                    $playlist_result = run_query("SELECT * FROM playlists WHERE id = '$next_id'");
                    $playlist = $playlist_result -> fetch_assoc();

                    $media_id = $playlist["media_id"];
                    $media_info_result = run_query("SELECT * FROM media WHERE id = '$media_id'");
                    $media_info = $media_info_result -> fetch_assoc();

                    $name = $media_info["file_name"];
                    $index = $index + 1;

                    echo "<tr>";
                    echo "<td>" . $index . "</td>";
                    echo "<td>" . $name . "</td>";
                    echo "<td><a href=view.php?playlist_id=" . $next_id . ">play</a></td>";
                    echo "<td><a href=remove_from_playlist.php?id=" . $next_id . ">remove</a></td>";
                    echo "<tr>";

                    $next_id = $playlist["next_id"];
                }
            ?>
        </table>

        <?php
            if (isset($_POST["rename_playlist"]))
            {
                $new_name = $_POST["name"];

                $check_playlists = run_query("SELECT * FROM playlists WHERE user_id = '$id' AND name = '$new_name'");
                $check_results = $check_playlists -> fetch_assoc();

                if ($check_results == 0)
                {
                    $user_id = get_current_user_id();
                    $current_name = $playlist_head["name"];
                    run_query("UPDATE playlists SET name = '$new_name' WHERE name = '$current_name' AND user_id = '$user_id'");
                    header("Refresh:0");
                }
                else
                {
                    echo "<div class='form_error'>You already have a playlist with that name!</div>";
                }
            }
        ?>

        <form id="rename_playlist_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
            <h4>Rename this playlist</h4>

            <div class="form_field">
                <label>New playlist name:</label>
                <input type="text" name="name" required>
            </div>

            <input name="rename_playlist" type="submit" value="Rename">
        </form>

        <?php
            }
        ?>
    </body>
</html>