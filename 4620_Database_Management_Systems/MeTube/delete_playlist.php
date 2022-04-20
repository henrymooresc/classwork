<?php
    include_once("navbar.php");
    include_once("helpers.php");

    $user_id = get_current_user_id();
    $playlist_head_id = $_GET["id"];

    $playlist_result = run_query("SELECT * FROM playlists WHERE id = '$playlist_head_id'");
    $playlist = $playlist_result -> fetch_assoc();
    $playlist_name = $playlist["name"];

    run_query("DELETE FROM playlists WHERE user_id = '$user_id' AND name = '$playlist_name'");
    header("Location: my_playlists.php");
?>