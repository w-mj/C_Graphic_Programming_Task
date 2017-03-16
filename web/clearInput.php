<?php
/**
 * Created by PhpStorm.
 * User: mj
 * Date: 17-3-13
 * Time: 下午8:14
 */

function sanitizeString($var)
{
    if (get_magic_quotes_gpc())
        $var = stripslashes($var);
    $var = htmlentities($var);
    $var = strip_tags($var);
    return $var;
}

?>