<?php

session_start();

require_once "clearInput.php";
echo <<< _HTML
<style>@import url('signup.css'); </style>
<style>@import url('cs.css'); </style>
<html><head><title>注册</title></head>
<body>
<div id='area'>
<h1>注册一个新用户</h1>
<form method="post" action="signup.php" >
用&nbsp;户&nbsp;名:<input class='inputLine' id='name' type="text" name="username" size="10" />
<br />
密&nbsp;&nbsp;码:<input class='inputLine' id='psd' type="password" name="password" size="10" />
<br />
重复密码:<input  class='inputLine' id='repeat' type="password" name="repeat" size="10" />
<br />
<input class='button' id='submitButton' type="submit" value="提交" />
</form>
_HTML;

static $notFirst = false;
$setu = isset($_POST['username']) && $_POST['username'] != '';
$setp = isset($_POST['password']) && $_POST['password'] != '';
$setr = isset($_POST['repeat']) && $_POST['repeat'] != '';

if($_SERVER['REQUEST_METHOD'] == "POST") {
    if ($setu == '')
        echo "<div class='text'> 用户名不能为空</div>";
    else if ($setp == '')
        echo "<div class='text'> 密码不能为空</div>";
    else if ($setr == '')
        echo "<div class='text'> 重复密码不能为空</div>";
    else {
        ($db = New mysqli('localhost', 'root', 'root', 'msgBoard')) or die('打开数据库失败');
        $db -> query("SET NAMES UTF-8");
        $db -> query("set character set 'utf8'");
        $input_username = $db->real_escape_string($_POST['username']);
        $input_password = $db->real_escape_string($_POST['password']);
        $input_repeatpsd = $db->real_escape_string($_POST['repeat']);

        $quarry = "SELECT * FROM userinfo WHERE username='$input_username'";
        ($result = $db->query($quarry)) or die('数据库查询失败');
        $arr = mysqli_fetch_array($result, MYSQLI_ASSOC);
        if ($arr) {
            echo "<div class='text'>用户名已被占用</div>";
        } elseif ($input_password != $input_repeatpsd)
            echo "<div class='text'>两次密码不匹配</div>";
        else {
            $quarry = "INSERT INTO userinfo(username, password) VALUES('$input_username', '$input_password')";
            $answer = $db->query($quarry);
            echo "<div class='text'>创建账户成功</div>";
            $notFirst = false;
            echo "<script>window.location.href='login.php'</script>";
        }
    }
}

echo "</div></body></html>";
?>
