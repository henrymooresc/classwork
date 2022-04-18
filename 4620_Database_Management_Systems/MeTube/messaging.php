<!-- Original file: MeTube-master/message.php -->

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

        <title>Messaging</title>
    </head>

    <body>
        <table>
            <caption>My Recieved Messages</caption>
            <tr>
                <th>Sender</th>
                <th>Message</th>
                <th>Reply?</th>
            </tr>
            <?php
                $id = get_current_user_id();
                $messages_result = run_query("SELECT * FROM messages WHERE to_id = '$id'");
                while ($row = $messages_result -> fetch_assoc())
                {
                    $from_id = $row["from_id"];
                    $from_user_info_result = run_query("SELECT * FROM users WHERE id = '$from_id'");
                    $from_user_info = $from_user_info_result -> fetch_assoc();

                    $content = $row["content"];
                    echo "<tr>";
                    echo "<td>" . $from_user_info["username"] . "</td>";
                    echo "<td>" . $content . "</td>";
                    echo "<td><a href=send_message.php?reply_id=" . $row["id"] . ">reply?</a></td>";
                    echo "</tr>";
                }
            ?>
        </table>

        <table>
            <caption>My Sent Messages</caption>
            <tr>
                <th>Recipient</th>
                <th>Message</th>
            </tr>
            <?php
                $id = get_current_user_id();
                $messages_result = run_query("SELECT * FROM messages WHERE from_id = '$id'");
                while ($row = $messages_result -> fetch_assoc())
                {
                    $to_id = $row["to_id"];
                    $to_user_info_result = run_query("SELECT * FROM users WHERE id = '$to_id'");
                    $to_user_info = $to_user_info_result -> fetch_assoc();

                    $content = $row["content"];
                    echo "<tr>";
                    echo "<td>" . $to_user_info["username"] . "</td>";
                    echo "<td>" . $content . "</td>";
                    echo "</tr>";
                }
            ?>
        </table>

        <a href="send_message.php"><button type="button">Send Message</button></a>
    </body>
</html>