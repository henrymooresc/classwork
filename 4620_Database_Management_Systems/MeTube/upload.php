<?php
    include_once "navbar.php";
?>

<!DOCTYPE html>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/style.css" />

        <style type="text/css">

        </style>

        <title>Upload Media</title>

        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    </head>

    <body>
        <div class="form_container">
            <form method="post" action="process_upload.php" enctype="multipart/form-data">
                <div class="form_field">
                    <label>File:</label>
                    <input name="file" type="file" required>
                </div>

                <div class="form_field">
                    <label>Title:</label>
                    <input type="text" name="title" required>
                </div>

                <div class="form_field">
                    <label>Category:</label>
                    <input type="text" name="category" requried>
                </div>

                <div class="form_field">
                    <label>Description:</label>
                    <input type="textarea" name="description" required>
                </div>

                <div class="form_field">
                    <label>Tags separated by spaces:</label>
                    <input type="text" name="tags" required>
                </div>
                    
                <input value="Upload" name="submit" type="submit">
            </form>
        </div>
    </body>
</html>
