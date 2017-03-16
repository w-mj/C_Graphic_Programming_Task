<?php
require_once 'clearInput.php';
session_start();
echo "<style>@import url('cs.css');</style>";
echo "<style>@import url('index.css');</style>";
echo '<html><meta charset="UTF-8" /><head><title>留言板</title></head>';
echo '<body>';

if(isset($_COOKIE['saveuser']) && (!isset($_SESSION['userid']) || $_SESSION['userid'] == ''))
    echo "<script>window.location.href='login.php'</script>";

$sql_user = 'msgboard';
$sql_password = 'root';
$sql_server = 'localhost';
$sql_database = 'msgBoard';

($db = New mysqli($sql_server, $sql_user, $sql_password, $sql_database)) or die('数据库连接失败');
$db -> query("SET NAMES UTF-8");
$db -> query("set character set 'utf8'");
($msg_result = $db ->query("SELECT * FROM message ORDER BY tm DESC")) or die('数据库读取失败');

//print_r($msg_result);
while($item = $msg_result -> fetch_assoc()) {
    $userid = $item['userid'];
    $tm = $item['tm'];
    $context = $item['context'];
    $public = $item['public'];
    $msgid = $item['msgid'];
    $query = "SELECT * FROM userinfo WHERE userid='$userid'";
    $uinfo_result = $db -> query($query);
    $userinfo = $uinfo_result -> fetch_assoc();
    $username = $userinfo['username'];
    echo "<div class='message'>";
    echo "<div class='msgHead' id='$msgid'>$username @ $tm</div>";
    echo "<textarea id='context' readonly>$context</textarea></div>";
    echo "<br /><br />";
}

echo "<br />";
if (isset($_SESSION['username']) && isset($_SESSION['userid'])) {
    $name = $_SESSION['username'];

    echo "<div id='tip'> $name ，请在此留言：</div>";
    echo "<div class='inputArea'>";
    echo "<form method='post' action='index.php'>";
    echo "<textarea class='inputBox' style='font-size: 120%;' name='commit'></textarea>";
    echo "<br />";
    echo "<input class='button' id='submitButton' type='submit' value='提交' / >";
    echo "</form>";
    echo "<form method='post' action='index.php'>";
    echo "<input type='hidden' name='signOut' value='1'>";
    echo "<input class='button' id='quitButton' type='submit' value='退出' / >";
    echo "</form></div>";
    if (isset($_POST['signOut']) && $_POST['signOut'] != '') {
        // 退出登录
        unset($_SESSION['userid']);
        unset($_SESSION['username']);
        setcookie('saveuser', -1 , time(), '/');
        echo '<script> location.replace(location.href); </script>';
    } elseif (isset($_POST['commit']) && $_POST['commit'] != '') {
        // 提交留言

        $userid = $_SESSION['userid'];
        $context = $db -> real_escape_string($_POST['commit']);

        $query = "INSERT INTO message(userid, tm, context, public, target) VALUES ($userid, SYSDATE(),'$context',1,0)";
        if (!$query) die ('插入数据失败');
        $db -> query($query);
        // $_POST['commit'] = '';
        unset($_POST['commit']);
        echo '<script> location.replace(location.href); </script>';
    }

} else {
    echo "<div class='inputArea'>";
    echo "<form  method='post'>";
    echo "<textarea class='inputBox' style='font-size: 120%;' readonly>只有登录后才能留言</textarea>";
    echo "<br />";
    echo "<input class='button' id='signInButton' type='submit' value='注册' formaction='signup.php'/ >";
    echo "<input class='button' id='signUpButton' type='submit' value='登录' formaction='login.php'/ >";
    echo "</form></div>";
}

echo "</body>";
echo "</html>"

?>
