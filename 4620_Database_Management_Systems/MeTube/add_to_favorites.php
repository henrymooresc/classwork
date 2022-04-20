<?php
    include_once("helpers.php");
    include_once("navbar.php");

    $user_id = get_current_user_id();
    $media_id = $_POST["media_id"];

    run_query("INSERT INTO favorites (user_id, media_id) VALUES ('$user_id', '$media_id')");

    header("Location: favorites.php");
?>