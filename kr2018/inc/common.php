<?php

define('NEWS_PER_PAGE',7);
define('NEWS_LENGTH',73);
define('URL','http://reasoning.eas.asu.edu/kr2018/');

function updatediv($div,$content)
{
	if($content=="")
	{
		echo"$('#$div').empty();";
		return;
	}

	$content = str_replace("\r","",$content);
	$content = str_replace("\n","",$content);
	$content = addslashes($content);

	echo"$('#$div').html(\"{$content}\");";
	
}

	
$now = mktime();
//$start = mktime(0, 0, 0, 7, 25, 2015);
//modified as conference approached
$start = mktime(0, 0, 0, 9, 10, 2015);

if($now<$start) $now = $start+5184000;


//NOT USED!!! SEE INDEX


$dates[0] = mktime(0, 0, 0, 11, 30, 2019);
$dates[1] = mktime(0, 0, 0, 11, 30, 2019);
$dates[2] = mktime(0, 0, 0, 1, 21, 2019);
$dates[3] = mktime(0, 0, 0, 2, 19, 2019);

$desc[0] = 'abstract submission';
$desc[1] = 'paper submission';
$desc[2] = 'author response';
$desc[3] = 'notification';
$desc[4] = 'camera ready';
$desc[5] = 'early bird registration ends';
$desc[6] = 'regular registration ends';
$desc[7] = 'workshops';
$desc[8] = 'conference';

?>
