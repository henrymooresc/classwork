<!DOCTYPE html>

<?php
	include_once "navbar.php";
	include_once "helpers.php";
    $id = get_current_user_id();
?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />

        <style type="text/css">

        </style>

        <title>Contacts</title>
    </head>

    <body>
        <table id="contacts_table">
            <caption>Contact List</caption>
            <tr>
                <th>Username</th>
                <th>Label</th>
            </tr>

            <?php
                $contacts_result = run_query("SELECT * FROM contacts WHERE user_id = '$id'");
                $contacts = $contacts_result -> fetch_all(MYSQLI_ASSOC);

                foreach ($contacts as $c)
                {
                    $contact_id = $c["contact_id"];
                    $contact_info_result = run_query("SELECT * FROM users WHERE id = '$contact_id'");
                    $contact_info = ($contact_info_result -> fetch_assoc());

                    echo "<tr>";
                    echo "<td>" . $contact_info["username"] . "</td>";
                    echo "<td>" . $c["label"] . "</td>";
                    echo "</tr>";
                }
            ?>
        </table>

        <br>

        <?php
            if (isset($_POST["submit_contact"]))
            {
                $contact_username = $_POST["new_contact"];
                $contact_info_result = run_query("SELECT * FROM users WHERE username = '$contact_username'");
                $contact_info = $contact_info_result -> fetch_assoc();

                if (!$contact_info == 0)
                {
                    $contact_id = $contact_info["id"];
                    $check_contacts = run_query("SELECT * FROM contacts WHERE user_id = '$id' AND contact_id = '$contact_id'");
                    $check_results = $check_contacts -> fetch_assoc();

                    if ($check_results == 0)
                    {
                        $label = $_POST["label"];
                        run_query("INSERT INTO contacts (label, user_id, contact_id) VALUES ('$label', '$id', '$contact_id')");
                        header("Refresh:0");
                    }
                    else
                    {
                        echo "<div class='form_error'>That user is already a contact</div>";
                    }
                }
                else
                {
                    echo "<div class='form_error'>A user with that name could not be found</div>";
                }
            }
        ?>

        <div class="form_container">
            <form id="add_contact_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <h4>Add a user contact</h4>

                <div class="form_field">
                    <label>Contact username:</label>
                    <input type="text" name="new_contact" required>
                </div>

                <div class="form_field">
                    <label>Contact label:</label>
                    <input type="text" name="label" required>
                </div>

                <input name="submit_contact" type="submit" value="Add User as Contact">
            </form>
        </div>

        <br>

        <?php
            if (isset($_POST["submit_remove"]))
            {
                $contact_username = $_POST["contact"];
                $contact_info_result = run_query("SELECT * FROM users WHERE username = '$contact_username'");
                $contact_info = $contact_info_result -> fetch_assoc();

                if (!$contact_info == 0)
                {
                    $contact_id = $contact_info["id"];
                    run_query("DELETE FROM contacts WHERE user_id = '$id' AND contact_id = '$contact_id'");
                    header("Refresh:0");
                }
                else
                {
                    echo "<div class='form_error'>A user with that name could not be found</div>";
                }
            }
        ?>

        <div class="form_container">
            <form id="remove_contact_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <h4>Remove a user contact</h4>

                <div class="form_field">
                    <label>Contact username:</label>
                    <input type="text" name="contact" required>
                </div>

                <input name="submit_remove" type="submit" value="Remove User as Contact">
            </form>
        </div>
    </body>
</html>