<?php
    include_once("navbar.php");
    include_once("helpers.php");

    $sub_id = $_GET["sub_id"];
    $sub_to_id = $_GET["sub_to_id"];

    run_query("DELETE FROM subscriptions WHERE sub_id = '$sub_id' AND sub_to_id = '$sub_to_id'");

    header("Location: channel.php?id='$sub_to_id'");
?>