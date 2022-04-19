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

        <title>My Playlists</title>
    </head>

    <body>
        <?php
            $id = get_current_user_id();
            $playlist_result = run_query("SELECT * FROM playlists WHERE user_id = '$id' AND first = 1");
        ?>

        <table id="my_playlists_table">
            <caption>My Playlists</caption>
            <tr>
                <th>Name</th>
                <th>View Playlist</th>
                <th>Delete?</th>
            </tr>
            <?php
                while ($entry = $playlist_result -> fetch_assoc())
                {
                    $name = $entry["name"];
                    $playlist_id = $entry["id"];
                
                    echo "<tr>";
                    echo "<td>" . $name . "</td>";
                    echo "<td><a href=playlist.php?id=" . $playlist_id . ">view</a></td>";
                    echo "<td><a href=delete_playlist.php?id=" . $playlist_id . ">delete</a></td>";
                    echo "<tr>";
                }
            ?>
        </table>

        <?php
            if (isset($_POST["create_playlist"]))
            {
                $playlist_name = $_POST["name"];

                $check_playlists = run_query("SELECT * FROM playlists WHERE user_id = '$id' AND name = '$playlist_name'");
                $check_results = $check_playlists -> fetch_assoc();

                if ($check_results == 0)
                {
                    run_query("INSERT INTO playlists (user_id, name, first) VALUES ('$id', '$playlist_name', 1)");
                    header("Refresh:0");
                }
                else
                {
                    echo "<div class='form_error'>You already have a playlist with that name!</div>";
                }
            }
        ?>

        <br>

        <div class="form_container">
            <form id="create_playlist_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <h4>Create a new playlist</h4>

                <div class="form_field">
                    <label>Playlist name:</label>
                    <input type="text" name="name" required>
                </div>

                <input name="create_playlist" type="submit" value="Create">
            </form>
        </div>
    </body>
</html>