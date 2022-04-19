<?php
    include_once "navbar.php";
    include_once "helpers.php";

    $user_id = get_current_user_id();
    $media_id = $_POST["media_id"];
    $comment = addslashes($_POST["comment"]);

    run_query("INSERT INTO comments (media_id, user_id, comment_text) VALUES ('$media_id', '$user_id', '$comment')");

    header("Location: view.php?id=$media_id");
?>