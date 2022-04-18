<!-- Original file: MeTube-master/login.php -->

<link rel="stylesheet" type="text/css" href="css/default.css" />

<?php
    include_once "navbar.php";
    include_once "helpers.php";

    if (isset($_POST['submit']))
    {
        $valid = true;

        if ($_POST['username'] == "")
        {
            $valid = false;
            echo "<div class='form_error'>A username is required</div>";
        }

        if ($_POST['password'] == "")
        {
            $valid = false;
            echo "<div class='form_error'>A password is required</div>";
        }
        
        if ($valid)
        {
            $result = login_auth($_POST["username"], $_POST["password"]);

            if (!$result)
            {
                echo "<div class='form_error'>Login information is incorrect</div>";
            }
            else
            {
                $_SESSION['username']=$_POST['username'];
                header('Location: homepage.php');
            }		
        }
    }
?>

<form method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>">
    <div class="form_field">
        <label>Username:</label>
        <input type="text" name="username" required>
    </div>

    <div class="form_field">
        <label>Password:</label>
        <input type="password" name="password" required>
    </div>

	<input name="submit" type="submit" value="Login">
</form>