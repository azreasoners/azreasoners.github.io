$().ready(function () {
    /*----- Preload Images -------------------------------------------------------*/

    $("img.MouseOver, input.MouseOver, img.Preload").each(function () {
        var imegeToLoad = $(this).attr("src").replace("_n.","_h.");
        jQuery("<img>").attr("src", imegeToLoad);
    });

    /*--- Add Mouseover Event Handler ----------------------------------------------*/
    $("img.MouseOver, input.MouseOver").hover(function () {
		if (!$(this).hasClass("Active")){
			$(this).attr("src", $(this).attr("src").replace("_n.","_h."));	
		}
    },function(){
		if (!$(this).hasClass("Active")){
			$(this).attr("src", $(this).attr("src").replace("_h.","_n."));	
		}
    });
});