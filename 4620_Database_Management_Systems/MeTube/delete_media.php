<?php
    include_once("navbar.php");
    include_once("helpers.php");

    $media_id = $_GET["id"];
    $my_id = get_current_user_id();

    $media_info_result = run_query("SELECT * FROM media WHERE id = '$media_id'");
    $media_info = $media_info_result -> fetch_assoc();

    if ($my_id == $media_info["uploader_id"])
    {
        run_query("DELETE FROM tags WHERE media_id = '$media_id'");
        run_query("DELETE FROM comments WHERE media_id = '$media_id'");
        run_query("DELETE FROM favorites WHERE media_id = '$media_id'");

        $playlists_result = run_query("SELECT * FROM playlists WHERE media_id = '$media_id'");

        while ($playlist = $playlists_result -> fetch_assoc())
        {
            $playlist_id = $playlist["id"];

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
        }

        unlink($media_info["path"]);

        run_query("DELETE FROM media WHERE id = '$media_id'");
        header("Location: channel.php?id=$my_id");
    }
?>