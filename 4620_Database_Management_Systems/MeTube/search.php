<?php
    include_once "navbar.php";
    include_once "helpers.php";

    $search_result_ids = array();
    $search_data = $_POST["search_data"];
    
    $title_result = run_query("SELECT id FROM media WHERE title LIKE '%$search_data%'");
    while ($entry = $title_result -> fetch_assoc())
    {
        array_push($search_result_ids, $entry["id"]);
    }

    $tag_result = run_query("SELECT media_id FROM tags WHERE tag LIKE '%$search_data%'");
    while ($entry = $tag_result -> fetch_assoc())
    {
        if (array_search($entry["media_id"], $search_result_ids) === false)
        {
            array_push($search_result_ids, $entry["media_id"]);
        }
    }

    $_SESSION["search_results"] = $search_result_ids;
    header("Refresh:0; url=browse.php");
?>

