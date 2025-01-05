
function ScrollTopWindow(page='',windowTop=false)
{
	var doc = document.documentElement;
	var top = (window.pageYOffset || doc.scrollTop)  - (doc.clientTop || 0);
	
	if(top<$("#page").offset().top-60) return;
	
	if(page!=='main' && !windowTop)
		$('html,body').animate({ scrollTop: $('#page').offset().top-60}, 'fast');
	else $('html,body').animate({ scrollTop: 0}, 'fast');
}



function setpage(page,menulink)
{
    $("#loader").show();
	$("#page").empty();
	
	
    $(".menulink").removeClass('selected');
			
   	$.post("ajax/showpage.php", {p:page, pp:menulink}, 
			function(data){  
				eval(data); 
				$("#loader").hide();
				ScrollTopWindow(page);
				
				return false;
	} );
	
	if(menulink===undefined) $("#"+page).addClass('selected');
	else $("#"+menulink).addClass('selected');
	
}



