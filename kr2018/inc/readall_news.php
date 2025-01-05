<?php

require_once "../inc/news.php";

$myXMLData = file_get_contents('../pages/news.xml');

$NewsReader = new newsReader();
$NewsReader->setTopic($param);

$NewsReader->setString($myXMLData);

$resp= "<h1>News</h1>".$NewsReader->print_all();


?>


