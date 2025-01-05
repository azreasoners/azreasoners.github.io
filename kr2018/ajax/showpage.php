<?php

require_once "../inc/common.php";


$page=$_POST['p'];
$lastmodified="";

if(!isset($_POST['p']))
	$page='main';

if (file_exists("../pages/page_{$page}.php"))
    $lastmodified = date ("d M Y", filemtime("../pages/page_{$page}.php"));

if($page=='main')
{
	$resp = file_get_contents("../pages/page_main.php");
	require_once "../inc/parse_news.php";

	$resp = "<div class='row'>
	<div id='page_left' class='col-md-11 col-sm-16' style='padding-left:0px; padding-right:0px;'>
	{$resp}
	</div>
	<div id='newsbox' class='col-md-5 col-sm-16'>
	{$news}
	</div>
	</div>";

	updatediv('page',$resp);
}
else if($page=='news')
{
	$param = $_POST['pp'];
	require_once "../inc/readall_news.php";
	updatediv('page',$resp);
}
else if(!file_exists("../pages/page_{$page}.php"))
{
	$resp = file_get_contents("../pages/404.php");
	$resp.="Requested page <b>{$page}</b> not found on the server.";

	updatediv('page',$resp);
}
else
{
	$resp = file_get_contents("../pages/page_{$page}.php");

	$resp = str_replace('<TBA>',"

		<div class='panel panel-warning'>
		  <div class='panel-heading'>
		    TBA
		  </div>

		</div>

	",$resp);

	$URL = URL;

	$resp = "
			<div id='thebuttons'>
				<span style='margin-right:6px; font-size:.8em;'>last modified: $lastmodified</span>

				<a href='javascript:void(0);' class='mybtn deeplink btn btn-default' role='button' data-placement='top' title='copy this to clipboard: {$URL}/?p={$page}' style='padding:3px 6px 3px 6px; color:#aaaaaa; '>
					<span class='glyphicon glyphicon-link' aria-hidden='true'></span>
				</a>
				<a href='javascript:window.print();' class='mybtn btn btn-default' role='button' data-placement='top' title='print content only' style='padding:3px 6px 3px 6px; color:#aaaaaa; '>
					<span class='glyphicon glyphicon-print' aria-hidden='true'></span>
				</a>
				<a href='javascript:void(0);' onclick=\"setpage('news','{$page}');\" class='mybtn btn btn-default' role='button' data-placement='top' title='related news' style='padding:3px 6px 3px 6px; color:#aaaaaa; '>
					<span class='glyphicon glyphicon-list' aria-hidden='true'></span>
				</a>&nbsp;&nbsp;&nbsp;
			</div>
			"
			.$resp;


	updatediv('page',"<div style='padding-right:10px'>{$resp}</div>");

	echo"$('#thebuttons > .mybtn').tooltip();";

	echo"$('.deeplink').bind('click', function(event) {

		//can't copy from dom. creating temp textarea
		var target = document.createElement('textarea');
		            target.style.position = 'absolute';
		            target.style.left = '-9999px';
		            target.style.top = '0';
		            target.id = 'temp';
		            document.body.appendChild(target);

		target.textContent = '{$URL}/?p={$page}';
		target.focus();
		target.setSelectionRange(0, target.value.length);

	    try {
	      var success = document.execCommand('copy');
	      if (success) {
	        $('.deeplink').trigger('copied', ['Copied!']);
	      } else {
	        $('.deeplink').trigger('copied', ['Copy with Ctrl-c']);
	      }
	    } catch (err) {
	      $('.deeplink').trigger('copied', ['Copy with Ctrl-c']);
	    }
	  });

	  $('.deeplink').bind('copied', function(event, message) {
	    $(this).attr('title', message)
	        .tooltip('fixTitle')
	        .tooltip('show')
	        .attr('title', '')
	        .tooltip('fixTitle');
	  });

	";
}

if($page=='papers')
{
?>

$('#filter').keyup(function () {

            var rex = new RegExp($(this).val(), 'i');
            $('span.paper').hide();
            $('span.paper').filter(function () {
                return rex.test($(this).text());
            }).show();

        })

<?php
}

if($page=='program')
{
	echo"

		var collapsed = true;

		$('#accordion-expand').click(function(){

		$('.session:not(:has(.highlight))').show();

		if(collapsed){ $('.collapse:not(\".in\")').collapse('show'); collapsed = false; $('#accordion-expand').text('collapse all sessions'); }
		else{ $('.collapse.in').collapse('hide'); collapsed = true;  $('#accordion-expand').text('expand all sessions'); }

	});";

	?>
	$('#filter').keyup(function () {

							$('div.program-paper').removeClass('highlight');

							//$('.collapse').collapse('show');

	            var rex = new RegExp($(this).val(), 'i');
	            //$('div.program-paper').hide();


							if($(this).val()=='')
							{
									$('div.program-paper').removeClass('highlight');//.show();
									$('#program-search-count').text('');
							}
							else
							{
									$('div.program-paper').filter(function () {
										return rex.test($(this).text());
									}).addClass('highlight');//.show();

									collapsed = false;
									$('#accordion-expand').text('collapse all sessions');

									//$(".collapse:not(:has('.highlight'))").collapse('hide');

									$('.collapse').filter('.collapse:not(:has(\'.highlight\'))').collapse('hide');
									$('.collapse').filter('.collapse:has(\'.highlight\')').collapse('show');

									$('#program-search-count').text('found: ' + $('div.program-paper').filter('.highlight').length);
							}



	        })

	<?php

	}
	?>
