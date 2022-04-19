<!-- Original file: MeTube-master/header.php -->

<!DOCTYPE html>

<?php
    session_save_path("sessions");

    if (!is_writable(session_save_path()))
    {
        echo "Session path '".session_save_path()."' is not writeable for PHP!";
    }

    session_start();
?>

<html>
    <head>
        <style type="text/css">

            .active {
                float: left !important;
            }

            #searchbar {
                padding-top: 5px;
                padding-right: 100px;
            }

            #nav ul {
                margin: 0;
                padding: 5px;
                list-style-type: none;
                text-align: center;
                background-color: #522D80;
                height: 45px;
            }

            #nav ul li {
                display: inline;
            }

            #nav ul li a {
                text-decoration: none;
                padding: .2em 1em;
                color: #fff;
                background-color: #522D80;
            }

            #nav ul li a:hover {
                background-color: #F66733;
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
                        echo "<li><a href='my_playlists.php'>Playlists</a></li>";
                        echo "<li><a href='favorites.php'>Favorites</a></li>";
                        echo "<li><a href='channel.php?id=" . $id . "'>Channel</a></li>";
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
