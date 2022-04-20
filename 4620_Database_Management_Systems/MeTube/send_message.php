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

        <?php
            if (isset($_GET["reply_id"]))
            {
                echo "<table>";
                echo "<caption>Replying to Message</caption>";
                echo "<tr>";
                echo "<th>Sender</th>";
                echo "<th>Message</th>";
                echo "</tr>";
            
                $reply_id = $_GET["reply_id"];
                $message_result = run_query("SELECT * FROM messages WHERE id = '$reply_id'");

                $message_info = $message_result -> fetch_assoc();
                $from_id = $message_info["from_id"];
                $from_user_info_result = run_query("SELECT * FROM users WHERE id = '$from_id'");
                $from_user_info = $from_user_info_result -> fetch_assoc();
                $content = $message_info["content"];

                echo "<tr>";
                echo "<td>" . $from_user_info["username"] . "</td>";
                echo "<td>" . $content . "</td>";
                echo "</tr>";
                echo "</table>";
            }

            if (isset($_POST["send_message"]))
            {
                $recipient = addslashes($_POST["recipient"]);

                $recipient_info_result = run_query("SELECT * FROM users WHERE username = '$recipient'");
                $recipient_info = $recipient_info_result -> fetch_assoc();

                if (!$recipient_info == 0)
                {
                    $id = get_current_user_id();
                    $recipient_id = $recipient_info["id"];
                    $check_contacts = run_query("SELECT * FROM contacts WHERE user_id = '$id' AND contact_id = '$recipient_id'");
                    $check_results = $check_contacts -> fetch_assoc();

                    if ($check_results == 0)
                    {
                        echo "<div class='form_error'>That user is not a contact</div>";
                    }
                    else
                    {
                        $content = addslashes($_POST["content"]);
                        run_query("INSERT INTO messages (to_id, from_id, content) VALUES ('$recipient_id', '$id', '$content')");
                        
                        unset($_POST["recipient"]);
                        unset($_POST["content"]);
                        header("Location: messaging.php");
                    }
                }
                else
                {
                    echo "<div class='form_error'>A user with that name could not be found</div>";
                }
            }
        ?>

        <div class="form_container">
            <form id="send_message_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <h4>Send Message</h4>    

                <div class="form_field">
                    <label>Send to:</label>
                    <input type="text" name="recipient" required>
                </div>

                <div class="form_field">
                    <label>Message content:</label>
                    <input type="text" name="content" required>
                </div>

                <input name="send_message" type="submit" value="Send Message">
            </form>
        </div>
    </body>
</html>