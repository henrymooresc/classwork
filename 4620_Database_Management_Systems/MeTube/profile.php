<!DOCTYPE html>

<?php
	include_once "navbar.php";
	include_once "helpers.php";

    $id = get_current_user_id();
    $user_info_result = run_query("SELECT * FROM users WHERE id = '$id'");
    $user_info = $user_info_result -> fetch_assoc();
?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />

        <style type="text/css">
            
        </style>

        <title>My Profile</title>
    </head>

    <body>
        <?php 
            if (isset($_POST["update_email"]))
            {
                $evalid = true;

                if (!filter_var($_POST["email"], FILTER_VALIDATE_EMAIL))
                {
                    $evalid = false;
                    echo "<div class='form_error'>Invalid email format</div>";
                }

                if ($evalid)
                {
                    $new_email = $_POST["email"];
                    
                    $q = "SELECT * FROM users WHERE email = '$new_email'";
                    $email_check_result = run_query($q);
                    $email_check = $email_check_result -> fetch_assoc();

                    if ($email_check == 0)
                    {
                        $q = "UPDATE users SET email = '$new_email' WHERE id = '$id'";
                        run_query($q);
                    }
                    else
                    {
                        echo "<div class='form_error'>Error, new email is already in use</div>";
                    }
                }
            }
        ?>

        <h2><?php echo $user_info["username"] ?>'s Profile Information</h2>

        <br>

        <div class="form_container">
            <form id="update_email_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <div class="form_field">
                    <label>Email Address:</label>
                    <input type="text" name="email" value=<?php echo $user_info["email"] ?> required>
                </div>

                <input name="update_email" type="submit" value="Update Email">
            </form>
        </div>

        <?php 
            if (isset($_POST["update_password"]))
            {
                $pvalid = true;

                if (!password_verify($_POST["current_pass"], $user_info["password"]))
                {
                    $pvalid = false;
                    echo "<div class='form_error'>Current password is invalid</div>";
                }
                else if ($_POST["new_pass1"] != $_POST["new_pass2"])
                {
                    $pvalid = false;
                    echo "<div class='form_error'>Passwords aren't the same</div>";
                }

                if ($pvalid)
                {
                    $new_pass_hash = password_hash($_POST["new_pass1"], PASSWORD_DEFAULT);

                    $q = "UPDATE users SET password = '$new_pass_hash' WHERE id = '$id'";
                    run_query($q);
                }
            }
        ?>

        <br>

        <div class="form_container">
            <form id="update_password_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <div class="form_field">
                    <label>Current Password:</label>
                    <input type="password" name="current_pass" required>
                </div>
                
                <div class="form_field">
                    <label>New Password:</label>
                    <input type="password" name="new_pass1" required>
                </div>

                <div class="form_field">
                    <label>New Password:</label>
                    <input type="password" name="new_pass2" required>
                </div>

                <input name="update_password" type="submit" value="Update Password">
            </form>
        </div>
    </body>
</html>