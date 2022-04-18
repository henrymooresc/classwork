<?php
    include_once("helpers.php");
    include_once("navbar.php");

    $user_id = get_current_user_id();
    $media_id = $_GET["id"];

    run_query("DELETE FROM favorites WHERE user_id = '$user_id' AND media_id = '$media_id'");

    header("Location: favorites.php");
?>