<?php
    session_start();
    
    $_SESSION = array();

    if (ini_get("session.use_cookies"))
    {
        $cookie_param = session_get_cookie_params();
        setcookie(session_name(), '', time() - 42000, $cookie_param["path"], $cookie_param["domain"], $cookie_param["secure"], $cookie_param["httponly"]);
    }
    
    session_destroy();
    header("Location: homepage.php");
?>
