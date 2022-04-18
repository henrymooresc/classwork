<?php
    include_once "navbar.php";
	include_once "helpers.php";

    $user_id = get_current_user_id();
    $media_id = $_POST["media_id"];
    $playlist_name = $_POST["playlist_name"];

    $playlist_check_result = run_query("SELECT * FROM playlists WHERE user_id = '$user_id' AND name = '$playlist_name' AND first = 1");
    $playlist_head = $playlist_check_result -> fetch_assoc();

    if (!$playlist_head == 0)
    {
        if (is_null($playlist_head["media_id"]))
        {
            $playlist_id = $playlist_head["id"];

            run_query("UPDATE playlists SET media_id = '$media_id' WHERE id = '$playlist_id'");
        }
        else
        {
            $playlist = $playlist_head;
            $media_id_here = $playlist["media_id"];

            if ($media_id == $media_id_here)
            {
                exit ("media already present in playlist");
            }

            $next_id = $playlist["next_id"];

            while (!is_null($next_id))
            {
                $playlist_result = run_query("SELECT * FROM playlists WHERE id = '$next_id'");
                $playlist = $playlist_result -> fetch_assoc();

                $media_id_here = $playlist["media_id"];

                if ($media_id == $media_id_here)
                {
                    exit ("media already present in playlist");
                }

                $next_id = $playlist["next_id"];
            }

            run_query("INSERT INTO playlists (user_id, media_id, name, first) VALUES ('$user_id', '$media_id', '$playlist_name', 0)");

            $new_playlist_result = run_query("SELECT * FROM playlists WHERE user_id = '$user_id' AND media_id = '$media_id' AND name = '$playlist_name'");
            $new_playlist = $new_playlist_result -> fetch_assoc();
            $new_id = $new_playlist["id"];
            $old_id = $playlist["id"];

            run_query("UPDATE playlists SET next_id = '$new_id' WHERE id = '$old_id'");

        }
        
        header("Location: playlist.php?id=" . $playlist_head["id"]);
    }
    else
    {
        echo "playlist by that name could not be found";
    }
?>