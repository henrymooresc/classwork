<!DOCTYPE html>

<?php
    session_save_path("sessions");

    if (!is_writable(session_save_path()))
    {
        echo "Session path '".session_save_path()."' is not writeable for PHP!";
    }

    session_start();

    include_once("helpers.php");
?>

<html>
    <head>
        <style type="text/css">

            #nav {
                background-color: #e03d3d;
                height: 60px;
            }

            #nav ul {
                padding: 0;
                margin: 0;
                list-style-type: none;
                overflow: hidden;
            }

            #nav li {
                float: left;
            }

            #nav ul li a {
                display: block;
                text-decoration: none;
                padding: 20px 5px;
                color: white;
                background-color: #e03d3d;
            }

            #nav ul li a:hover {
                background-color: #e31414;
            }

            #searchbar {
                padding: 20px 0 20px 150px;
            }

        </style>
    </head>

    <body>
        <div id="nav">
            <ul>
                <li><a href="homepage.php">MeTube Homepage</a></li>

                <?php 
                    if (isset($_SESSION["username"]))
                    {
                        $id = get_current_user_id();
                        echo "<li><a href='channel.php?id=" . $id . "'>My Channel</a></li>";
                        echo "<li><a href='my_playlists.php'>Playlists</a></li>";
                        echo "<li><a href='favorites.php'>Favorites</a></li>";
                        echo "<li><a href='profile.php'>Profile</a></li>";
                        echo "<li><a href='contacts.php'>Contacts</a></li>";
                        echo "<li><a href='messaging.php'>Messaging</a></li>";
                        echo "<li><a href='logout.php'>Logout</a></li>";
                    }
                    else 
                    {
                        echo "<li><a href='login.php'>Login</a></li>";
                        echo "<li><a href='register.php'>Register</a></li>";
                    }
                ?>

                <li>
                    <form id="searchbar" action="search.php" method="post">
                        <input type="text" placeholder="search our media" name="search_data">
                        <input type="submit" value="Search">
                    </form>
                </li>
            </ul>
        </div>
    </body>
</html>
