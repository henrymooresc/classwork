<?php
    $host = "mysql1.cs.clemson.edu";
    $user = "mtbprmry_006u";
    $pass = "theriverruns@then1ghT";
    $db = "metube_primary_sta9";

    $conn = new mysqli($host, $user, $pass, $db);

    if ($conn -> connect_errno)
    {
        exit("database connection failed: " . $conn -> connect_error);
    }

    function register_user ($username, $password, $email)
    {
        global $conn;
        $q = "SELECT * FROM users WHERE username = '$username' OR email = '$email'";
        $user_check_result = $conn -> query($q);

        if (!$user_check_result) 
        {
            exit ("register_user username check query failed: " . $conn -> error);
        }
        else 
        {
            $user_check = $user_check_result -> fetch_assoc();

            if ($user_check == 0)
            {
                $hash = password_hash($password, PASSWORD_DEFAULT);
                $q = "INSERT INTO users (type, username, password, email) VALUES ('1','$username','$hash', '$email')";
                echo $q;
                
                return $conn -> query($q);
            }
            else
            {
                return false;
            }
        }
    }

    function login_auth($username, $password)
    {
        global $conn;
        $q = "SELECT password FROM users WHERE username='$username'";
        $result = $conn -> query($q);

        if (!$result)
        {
            exit ("login_auth query failed: <br />" . $conn -> error);
        }
        else
        {
            $row = $result -> fetch_row();
            return password_verify($password, $row[0]);
        }	
    }

    function run_query($q)
    {
        global $conn;
        $result = $conn -> query($q);

        return $result;
    }

    function get_current_user_id()
    {
        global $conn;
        $username = $_SESSION["username"];

        $result = $conn -> query("SELECT id FROM users WHERE username = '$username'");
        $id = $result -> fetch_row();

        return $id[0];
    }
?>
