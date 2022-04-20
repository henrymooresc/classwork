<?php
    include_once "helpers.php";

    $id = $_GET["id"];

    $result = run_query("SELECT * FROM media WHERE id = " . $id);
    $media_data = $result -> fetch_assoc();

    $file_name = $media_data["file_name"];
    $type = $media_data["type"];
    $path = $media_data["path"];

    header("Content-disposition: attachment; filename=" . $file_name);
    header("Content-type: " . $type);
    readfile($path);
?>