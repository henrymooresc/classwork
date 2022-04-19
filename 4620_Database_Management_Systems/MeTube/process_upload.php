<?php
	include_once "navbar.php";
	include_once "helpers.php";

    $username = $_SESSION["username"];

    $directory = "uploads/" . $username . "/";

    if (!file_exists($directory))
    {
        mkdir($directory, 0775);
    }
    else
    {
        chmod($directory, 0775);
    }

    if ($_FILES["file"]["error"] > 0)
    {
        $result = $_FILES["file"]["error"];
    }
    else
    {
        $upload_path = $directory.urlencode($_FILES["file"]["name"]);
        
        if (file_exists($upload_path))
        {
            $result = 5;
        }
        else
        {
            if (is_uploaded_file($_FILES["file"]["tmp_name"]))
            {
                if (!move_uploaded_file($_FILES["file"]["tmp_name"], $upload_path))
                {
                    $result = 6;
                }
                else
                {
                    $title = addslashes($_POST["title"]);
                    $description = addslashes($_POST["description"]);
                    $category = addslashes($_POST["category"]);
                    $user_id = get_current_user_id();
                    $type = substr($_FILES["file"]["type"], 0, 5);

                    $query = "INSERT INTO media (file_name, type, path, title, description, category, view_count, uploader_id)" . "VALUES ('" . urlencode($_FILES["file"]["name"]) . "', '" . $type . "', '" . $upload_path . "', '" . $title . "', '" . $description . "', '" . $category . "', " . 0 . ", '" . $user_id . "')";
                    
                    run_query($query);

                    chmod($upload_path, 0644);
                    
                    $result = run_query("SELECT id FROM media WHERE path = '" . $upload_path . "'");
                    $row = $result -> fetch_assoc();
                    $media_id = $row["id"];

                    $tags = explode(' ', $_POST['tags']);

                    foreach ($tags as $tag)
                    {
                        $tag = trim($tag);
                        run_query("INSERT INTO tags (media_id, tag) VALUES ('" . $media_id . "', '" . $tag . "')");
                    }

                    $result = 0;
                }
            }
            else  
            {
                $result = 7;
            }
        }
    }

    if (isset($result))
    {
        if ($result === 0)
        {
            header("Location: view.php?id=$media_id");
        }
        else if ($result === 1)
        {
            echo "<h2>Uploading error, file size exceeds maximum<h2>";
        }
        else if ($result === 2)
        {
            echo "<h2>Uploading error, file size exceeds maximum</h2>";
        }
        else if ($result === 3)
        {
            echo "<h2>Uploading error, file was only partially uploaded</h2>";
        }
        else if ($result === 4)
        {
            echo "<h2>Uploading error, no file was uploaded</h2>";
        }
        else if ($result === 5)
        {
            echo "<h2>Uploading error, file already exists</h2>";
        }
        else if ($result === 6)
        {
            echo "<h2>Uploading error, file could not be moved from tmp directory to uploads</h2>";
        }
        else if ($result === 7)
        {
            echo "<h2>Uploading error, file write to disk failed<h2>";
        }
        else
        {
            echo "<h2>Something else broke but I'm not sure what!</h2>";
        }
    }
?>

