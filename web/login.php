<?php
require_once 'clearInput.php';
session_start();

echo <<< _HTML
<html><head><title>登录</title></head>
<body">
<style>@import url('login.css');</style>
<style>@import url('cs.css');</style>
<div class='signIn'>
<h1>登录你的账号</h1>
<form method="post" action="login.php">
用户名:<input class='inputLine' type="text" name="username" size="10" />
<br />
 密 码:<input class='inputLine' id='psdLine' type="password" name="password" size="10" />
 <br />
 <input class='button' id='login_submit' type="submit" value="登录" />
 <input class='button' id='signUp_submit' type='submit' formaction='signup.php' value='注册' />
</form>

_HTML;
// session直接连接
if (isset($_SESSION['username']) && isset($_SESSION['userid']) && $_SESSION['userid']!='' && $_SESSION['username']!='')
    echo "<script>window.location.href='index.php'</script>";
// 找到了cookie
elseif (isset($_COOKIE['saveuser']) && $_COOKIE['saveuser'] != '') {
    ($db = New mysqli('localhost', 'msgboard', 'root', 'msgBoard')) or die('数据库连接失败');
    $db -> query("SET NAMES UTF-8");
    $db -> query("set character set 'utf8'");
    $userid = $_COOKIE['saveuser'];
    $query = "SELECT * FROM userinfo WHERE userid='$userid'";
    $result = $db -> query($query) or die('数据库查询失败');
    $arr = mysqli_fetch_array($result, MYSQLI_ASSOC);
    if ($arr) {
        echo "<div class='text'>登录成功</div>";
        $_SESSION['username'] = $arr['username'];
        $_SESSION['userid'] = $arr['userid'];
        echo "<script>window.location.href='index.php'</script>";
    } else {
        // cookie在数据库中并不存在，清除cookie
        setcookie('saveuser', -1 , time(), '/');
    }
}
// 输入密码
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST['username']) && $_POST['username'] != ''
        && $_POST['password'] && $_POST['password'] != ''
    ) {
        //print_r($_POST);
        $input_username = sanitizeString($_POST['username']);
        $input_password = sanitizeString($_POST['password']);
        unset($_POST['username']);
        unset($_POST['password']);

        ($db = New mysqli('localhost', 'root', 'root', 'msgBoard')) or die('数据库连接失败');

        $query = "SELECT * FROM userinfo WHERE username='$input_username'";

        $result = $db->query($query) or die('数据库查询失败');

        $arr = mysqli_fetch_array($result, MYSQLI_ASSOC);

        if (!$arr)
            echo "<div class='text'>无此用户</div>";
        else {
            if ($arr['password'] == $input_password) {
                echo "<div class='text'>登录成功</div>";
                $_SESSION['username'] = $input_username;
                $_SESSION['userid'] = $arr['userid'];
                setcookie('saveuser', $arr['userid'], time() + 604800, '/');
                echo "<script>window.location.href='index.php'</script>";
            } else
                echo "<div class='text'>用户名或密码错误</div>";
        }
    } else {
        echo "<div class='text'>用户名和密码不能为空</div>";
    }
}

echo "</div></body></html>";
?>
