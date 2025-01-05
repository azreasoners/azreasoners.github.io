<?php

require_once "inc/news.php";

$myXMLData = file_get_contents('pages/news.xml');

$NewsReader = new newsReader();
$NewsReader->setString($myXMLData,NEWS_PER_PAGE);
echo $NewsReader->print_page(1,NEWS_LENGTH);

?>


