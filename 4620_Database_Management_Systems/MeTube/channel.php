<!DOCTYPE html>

<?php
	include_once "navbar.php";
	include_once "helpers.php";
?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />
        <style type="text/css">

        </style>

        <title>Channel</title>
    </head>

    <body>
        <?php
            $channel_id = $_GET["id"];
            $my_id = get_current_user_id();

            $media_result = run_query("SELECT * FROM media WHERE uploader_id = '$channel_id'");
            $channel_owner_result = run_query("SELECT * FROM users WHERE id = '$channel_id'");
            $channel_owner = $channel_owner_result -> fetch_assoc();

            $media_count_result = run_query("SELECT COUNT(id) AS num FROM media WHERE uploader_id = '$channel_id'");
            $media_count = $media_count_result -> fetch_assoc();

            $subscriber_count_result = run_query("SELECT COUNT(id) AS num FROM subscriptions WHERE sub_to_id = '$channel_id'");
            $subscriber_count = $subscriber_count_result -> fetch_assoc();

            if (!$media_result)
            {
                exit ("Could not query the media table in the database");
            }
        ?>

        <h3>Welcome to <?php echo $channel_owner["username"] ?>'s Channel!</h3>

        <?php
            if ($channel_id !== $my_id)
            {
                $subscribed_check_result = run_query("SELECT * FROM subscriptions WHERE sub_id = '$my_id' AND sub_to_id = '$channel_id'");
                $subscribed_check = $subscribed_check_result -> fetch_assoc();

                if ($subscribed_check == 0)
                {
                    echo "<a href=add_subscription.php?sub_id=$my_id&sub_to_id=$channel_id><button type='button'>Subscribe?</button></a>";
                }
                else
                {
                    echo "<a href=delete_subscription.php?sub_id=$my_id&sub_to_id=$channel_id><button type='button'>UnSubscribe?</button></a>";
                }
            }
        ?>

        <table>
            <caption>Channel Stats</caption>
            <tr>
                <th>Total Media Uploaded</th>
                <th>Total Subscriber Count</th>
            </tr>
            <tr>
                <?php
                    echo "<td>" . $media_count["num"] . "</td>";
                    echo "<td>" . $subscriber_count["num"] . "</td>";
                ?>
            </tr>
        </table>

        <table>
            <caption>Media Results</caption>
            <tr>
                <th>Title</th>
                <th>Uploader</th>
                <th>Date Uploaded</th>
                <th>Media Type</th>
                <th>Category</th>
                <th>Total Views</th>
                <?php
                    if ($channel_id == $my_id)
                    {
                        echo "<th>Delete Media?</th>";
                    }
                ?>
            </tr>
            <?php
                while ($entry = $media_result -> fetch_assoc())
                {
                    $media_id = $entry["id"];
                    $file_type = $entry["type"];
                    $upload_date = $entry["upload_date"];
                    $title = $entry["title"];
                    $category = $entry["category"];
                    $view_count = $entry["view_count"];
                    $uploader_id = $entry["uploader_id"];

                    $username_result = run_query("SELECT username FROM users WHERE id = " . $uploader_id);
                    $uploader = ($username_result -> fetch_row())[0];
                
                    echo "<tr>";
                    echo "<td>" . $title . "</td>";
                    echo "<td>" . $uploader . "</td>";
                    echo "<td>" . $upload_date . "</td>";
                    echo "<td>" . $file_type . "</td>";
                    echo "<td>" . $category . "</td>";
                    echo "<td>" . $view_count . "</td>";
                    echo "<td><a href=view.php?id=" . $media_id . ">view</a></td>";

                    if ($channel_id == $my_id)
                    {
                        echo "<td><a href=delete_media.php?id=" . $media_id . ">delete</a></td>";
                    }

                    echo "<tr>";
                }
            ?>
        </table>
    </body>
</html>