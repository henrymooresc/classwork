<?php
    include_once "navbar.php";
	include_once "helpers.php";

    $user_id = get_current_user_id();
    $playlist_id = $_GET["id"];

    $playlist_result = run_query("SELECT * FROM playlists WHERE id = '$playlist_id'");
    $playlist = $playlist_result -> fetch_assoc();

    $playlist_name = $playlist["name"];

    if ($playlist["first"] == 1)
    {
        if (is_null($playlist["next_id"]))
        {
            run_query("UPDATE playlists SET media_id = NULL WHERE id = '$playlist_id'");
        }
        else
        {
            $new_head_id = $playlist["next_id"];
            run_query("UPDATE playlists SET first = 1 WHERE id = '$new_head_id'");
            run_query("DELETE FROM playlists WHERE id = '$playlist_id'");
        }
    }
    else
    {
        $parent_playlist_result = run_query("SELECT * FROM playlists WHERE next_id = '$playlist_id'");
        $parent_playlist = $parent_playlist_result -> fetch_assoc();
        $parent_id = $parent_playlist["id"];
        $next_id = $playlist["next_id"];

        if (is_null($next_id))
        {
            run_query("UPDATE playlists SET next_id = NULL WHERE id = '$parent_id'");
            run_query("DELETE FROM playlists WHERE id = '$playlist_id'");
        }
        else
        {
            run_query("UPDATE playlists SET next_id = '$next_id' WHERE id = '$parent_id'");
            run_query("DELETE FROM playlists WHERE id = '$playlist_id'");
        }
    }

    $playlist_head_result = run_query("SELECT * FROM playlists WHERE user_id = '$user_id' AND name = '$playlist_name' AND first = 1");
    $playlist_head = $playlist_head_result -> fetch_assoc();
    $head_id = $playlist_head["id"];

    header("Location: playlist.php?id=" . $head_id);
?>