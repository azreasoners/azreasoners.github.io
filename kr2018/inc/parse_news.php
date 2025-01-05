<?php

$news = "<h2>News</h2><div id='news' style='min-height:95px'>";

require_once "../inc/news.php";

$myXMLData = file_get_contents('../pages/news.xml');

$NewsReader = new newsReader();
$NewsReader->setString($myXMLData,NEWS_PER_PAGE);
$news.= $NewsReader->print_page();


$news.= "</div>";
$news.="<br/><div style='text-align:right;'><a href='javascript:void(0);' style='font-weight:normal;' onclick=\"setpage('news');\"><i>read all</i></a></div>";


?>


