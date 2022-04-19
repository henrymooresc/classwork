<!DOCTYPE html>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />

        <style type="text/css">
            
        </style>

        <title>Register</title>
    </head>

    <body>
        <?php
            include_once "navbar.php";
            include_once "helpers.php";

            if (isset($_POST["submit"]))
            {
                $valid = true;

                if (!filter_var($_POST["email"], FILTER_VALIDATE_EMAIL))
                {
                    $valid = false;
                    echo "<div class='form_error'>Invalid email format</div>";
                }

                if ($_POST["pass1"] != $_POST["pass2"])
                {
                    $valid = false;
                    echo "<div class='form_error'>Passwords aren't the same</div>";
                }

                if ($valid)
                {
                    $result = register_user($_POST["username"], $_POST["pass1"], $_POST["email"]);	

                    if ($result)
                    {
                        $_SESSION["username"] = $_POST["username"];

                        header("Location: homepage.php");
                    }
                    else
                    {
                        echo "<div class='form_error'>Username is taken</div>";
                    }
                }
            }
        ?>

        <div class="form_container">
            <form id="register_form" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" method="post">
                <div class="form_field">
                    <label>Email Address:</label>
                    <input type="text" name="email" required>
                </div>
                
                <div class="form_field">
                    <label>Username:</label>
                    <input type="text" name="username" required>
                </div>
                
                <div class="form_field">
                    <label>Create Password:</label>
                    <input type="password" name="pass1" required>
                </div>
                
                <div class="form_field">
                    <label>Confirm Password:</label>
                    <input type="password" name="pass2" required>
                </div>
            
                <input name="submit" type="submit" value="Submit">
            </form>
        </div>
    </body>
</html>
