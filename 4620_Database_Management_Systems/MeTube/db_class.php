<!-- Original file: MeTube-master/mysqlClass.inc.php -->

<?php
    class db {
        var $connection;
        var $result_data;
        var $row = array();
        var $rowset = array();

        function connect_db($host, $user, $pass, $db_name) {
            $this->host = $host;
            $this->user = $user;
            $this->password = $pass;
            $this->db_name = $db_name;

            $this->connection = mysqli_connect($this->host, $this->user, $this->password, $this->db_name);

            return $this->connection;
        }

        function close() {
            if($this->connection) {

                if($this->result_data) {
                    mysqli_free_result($this->result_data);
                }

                return mysqli_close($this->connection);

            } else {
                return false;
            }
        }

        function get_row($query_id = 0) {
            if(!$query_id) {
                $query_id = $this->result_data;
            }
            if($query_id) {
                $this->row[$query_id] = mysqli_fetch_array($query_id);
                return $this->row[$query_id];
            } else {
                return false;
            }
        }

        function get_field($query_id = 0, $field) {
            if(!$query_id) {
                $query_id = $this->result_data;
            }
            if($query_id) {
                $this->row[$query_id] = mysqli_fetch_field($query_id);
                return $this->row[$query_id];
            } else {
                return false;
            }
        }

        function get_num_fields($query_id = 0) {
            if(!$query_id) {
                $query_id = $this->result_data;
            }
            if($query_id) {
                $result = @mysqli_num_fields($query_id);
                return $result;
            } else {
                return false;
            }
        }

        function get_num_rows($query_id = 0) {
            if(!$query_id) {
                $query_id = $this->result_data;
            }
            if($query_id) {
                $result = @mysqli_num_rows($query_id);
                return $result;
            } else {
                return false;
            }
        }

        function query_data($query = "") {
            unset($this->result_data);
            if($query != "") {
                $this->result_data = mysqli_query($this->connection, $query);
            }

            if($this->result_data) {
                unset($this->row[$this->result_data]);
                unset($this->rowset[$this->result_data]);
                return $this->result_data;
            } else {
                return false;
            }
        }
    }
?>
