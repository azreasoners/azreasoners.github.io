<?php

require_once("../inc/common.php");
require_once("../inc/news.php");

$myXMLData = file_get_contents('../pages/news.xml');

$NewsReader = new newsReader();
$NewsReader->setString($myXMLData,NEWS_PER_PAGE);

$NewsReader->setTopic($topic);
$resp = $NewsReader->print_page();

updatediv('news',$resp);


?>
