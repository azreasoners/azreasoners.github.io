<!-- Portfolio -->

<script>
var slideIndex = 1;
var slideIndex1 = 1;
showDivs(slideIndex);
showDivs1(slideIndex1);
function plusDivs(n) {
  showDivs(slideIndex += n);
}

function plusDivs1(n) {
  showDivs1(slideIndex1 += n);
}

function showDivs(n) {
  var i;
  var x = document.getElementsByClassName("mySlides");
  if (n > x.length) {slideIndex = 1}    
  if (n < 1) {slideIndex = x.length}
  for (i = 0; i < x.length; i++) {
     x[i].style.display = "none";  
  }
  x[slideIndex-1].style.display = "block";  
}
function showDivs1(n) {
  var i;
  var x = document.getElementsByClassName("mySlides1");
  if (n > x.length) {slideIndex1 = 1}    
  if (n < 1) {slideIndex1 = x.length}
  for (i = 0; i < x.length; i++) {
     x[i].style.display = "none";  
  }
  x[slideIndex1-1].style.display = "block";  
}


</script>

<h1>Venue</h1>

<ul class='arrows'>
<li><a href='#conference'>Conference venue</a></li>
<li><a href='#workshops'>Workshops venue</a></li>
<li>
	<a href='#travelinfo'>Travel information</a>
	<ul class='arrows'>
		<li>Airport Shuttle Information</li>
		<li>Directions to the hotel</li>
	</ul>
</li>
<li><a href='#foodinfo'>Food information</a></li>
</ul>



<p id='conference'></p>	
<br/><br/>
<h2>CONFERENCE VENUE</h2>

<p>KR 2018 main conference will be held at the:</p>

<p>
	<b>Tempe Mission Palms Hotel and Conference Center</b><br/>
	<a href='https://www.destinationhotels.com/tempe-mission-palms' target='_blank' title='Tempe Mission Palms Hotel'>hotel website <span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
	60 E 5th St</br>
	Tempe, AZ 85281</br>
	United States</br>
	Phone: +1 (480) 894-1400
</p>


<div class='panel panel-warning'>
	<div class='panel-heading' style='padding:10px; color:black;'>
<p>KR/DL/NMR 2018 attendants will receive the <b>discounted rate</b>:</p>

	<p><b>Single and Double</b>: $179/night<br/>
	$12.95/night hospitality fee to cover airport transportation, values or self-parking, access to the health club and fitness center, local and toll-free telephone calls, high-speed wireless internet access, incoming fax service, daily newspaper, and in-room coffee service.</p>
	
	<p>The <b>cut-off date</b> is October 1st, 2018 but the sooner the better because rooms may be sold out during the attractive season for visiting Arizona. <br/></p>

	<p>In order to receive discounted rate, please use following link for hotel reservation:<br/> 
		<a target="_blank" href="https://gc.synxis.com/rez.aspx?Hotel=12318&Chain=15564&arrive=10/29/2018&depart=11/3/2018&adult=1&child=0&group=2UF35H">KR2018 Guest Link <span class="glyphicon glyphicon-link" aria-hidden="true"></span></a></p>
		
	<p>In case you want to phone in, the toll-free number for reservation is: <b>800-547-8705</b><br/>
	When you reserve the room, please provide the group code: <b>2UF35H</b></p>
		</div>
	</div>

  
</div>

<div class="w3-content w3-display-container">
  <img class="mySlides" src="img/gal/hotel/1.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/2.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/3.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/4.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/5.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/6.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/7.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/8.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/9.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/10.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/11.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/12.jpg" style="width:100%">
  <img class="mySlides" src="img/gal/hotel/13.jpg" style="width:100%">
  <button class="w3-button w3-black w3-display-left" onclick="plusDivs(-1)">&#10094;</button>
  <button class="w3-button w3-black w3-display-right" onclick="plusDivs(1)">&#10095;</button>
</div>



<p id='workshops'></p>	
<br/><br/>
<h2>WORKSHOPS VENUE</h2>


<p>KR 2018 workshops and tutorials session will be held at the:</p>

<p>
	<b>ASU Memorial Union </b><br/>
	<a href='https://eoss.asu.edu/mu' target='_blank' title='ASU Memorial Union'>website <span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
	301 E Orange St</br>
	Tempe, AZ 85281</br>
	United States</br>
	Phone: +1 (480) 965-5728
<p/>

<div class="w3-content w3-display-container">
  <img class="mySlides1" src="img/gal/union/1.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/2.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/3.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/4.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/5.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/6.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/7.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/8.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/9.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/10.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/11.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/12.jpg" style="width:100%">
  <img class="mySlides1" src="img/gal/union/13.jpg" style="width:100%">
  <button class="w3-button w3-black w3-display-left" onclick="plusDivs1(-1)">&#10094;</button>
  <button class="w3-button w3-black w3-display-right" onclick="plusDivs1(1)">&#10095;</button>
</div>


<p id='travelinfo'></p>	
<br/><br/>
<h2>TRAVEL INFORMATION</h2>

<div>
    <div>
	<iframe src="https://www.google.com/maps/d/u/0/embed?mid=17KtMkDK9ytAes9VLAvtSk5-XJfZnjuDI" width="640" height="480"></iframe>	
	
	<p>Tempe Mission Palms Hotel, where KR 2018 main conference will be held, is conveniently located near Arizona State University (ASU), and just 10 minutes from Phoenix Sky Harbor International Airport and all major highways in and around Phoenix and Scottsdale. It is also located within walking distance to downtown Tempe's shopping, dining and entertainment district.</p>
	
	<p>The Memorial Union, considered the living room of the campus, will be holding KR 2018 workshops and tutorials sessions. It has a variety of activities and dining choices. Please visit <a href='https://eoss.asu.edu/mu'target='_blank'>MU website</a> for more information. </p>
	


<h2>Airport Shuttle Information</h2>
	<p>The hotel provides daily shuttle transportation <b>from 5:30am to 10:00pm</b></p>
	
	<p><b>To the hotel:</b><br/> 
	When you arrive at baggage claim and have luggage in-hand, please call our hotel at +1 (480) 894-1400. The operator will direct you to the proper curbside location for your terminal, and advise you of the estimated pick up time.</p>
	
	<p><b>To the airport:</b><br/> 
	The airport shuttle from hotel near Phoenix airport runs on an "as-needed" basis, leaving every 30 minutes, on the hour and half-hour. Reservations are not required.<p/>
	
	<div class='panel panel-warning'>
	<div class='panel-heading' style='padding:10px; color:black;'>
	Due to variations in scheduled versus actual arrival times, the hotel does not accept "Pre-schedule" shuttle pick-ups
	</div>
	</div>
	
	
<h2>Directions to the hotel</h2>

	
	<p><b>From Sky Harbor International Airport:</b></p>

	<ul class='arrows'>
	<li>Follow airport signs to East Exits</li>  
    	<li>Continue to follow signs for AZ-202 E and merge onto E Sky Harbor Blvd</li>  
 	<li>Keep right at the fork, follow signs for Priest Dr/Center Pkwy</li>  
   	<li>Turn right onto Priest Dr</li> 
	<li>Turn left onto W Rio Salado Pkwy</li>
	<li>Continue onto S Ash Ave</li>
	<li>Turn left onto W 5th Street, hotel will be on your left</li>
    	</ul>

	<p><b>From Sky Harbor International Airport Rental Car Center:</b></p>
	<ul class='arrows'>
	<li>Head south toward E Sky Harbor Cir S</li>  
    	<li>Turn left onto E Sky Harbor S</li>  
 	<li>Turn right onto E Buckeye Rd</li>  
   	<li>Merge onto I-10 E via the ramp to I-17N</li> 
	<li>Take exit 153 for 48th St toward AZ-143/Broadway Rd</li>
	<li>Keep left at the fork, follow signs for AZ-143N/Broadway Rd</li>
	<li>Turn left onto S 48th Street</li>
	<li>Take exit 2 for University Dr</li>
	<li>Turn right onto University Dr</li>
	<li>Turn left onto S Mill Ave</li>
	<li>Turn right onto E 5th St, hotel will be on your left</li>
    	</ul>

    </div>
 </div>




 <p id='foodinfo'></p>	
<br/><br/>
<h2>FOOD INFORMATION</h2>

<div>
    <div>

    <p> Lunch is not provided during the conference, but there are quite a lot of good restaurants around the venue. You can find a restaurant that suits your taste through <a href='https://www.yelp.com/'target='_blank'>Yelp</a>.</p>

    <p> For your convenience, here we list some restaurants that are popular and include vegetarian meal on their menus. Their locations are also pinned at the end. </p>
	
	<p><b>Restaurants with good environments:</b></p>

	<ul class='arrows'>
	<li><a href='https://www.houseoftricks.com/'target='_blank'>House of Tricks</a> -- Cozy spot serving <b>New American fare</b> in a 1920s bungalow with a patio featuring a fire pit & bar.</li>  
 	<li><a href='https://www.pfchangs.com/locations/us/az/tempe/740-s-mill-ave/2800-tempe.html?utm_source=YextMain&utm_medium=Directory'target='_blank'>P.F. Chang's</a> -- Family-friendly chain offering creative takes on <b>Chinese fare</b> in a striking space.</li>  
   	<li><a href='http://www.pitajungle.com/locations/tempe-mill/'target='_blank'>Pita Jungle</a> -- Chain for health-oriented <b>Mediterranean dishes</b>, including lots of veggie options, in sleek environs.</li> 
    </ul>

    <p><b>Restaurants without good environments:</b></p>

	<ul class='arrows'>
	<li><a href='https://www.houseoftricks.com/'target='_blank'>The Chuckbox</a> -- Basic, <b>cash-only </b> <b>burger</b> joint turning out <b>charcoal-broiled patties</b> plus beer & wine.</li>  
 	<li><a href='https://www.mkt.com/desertrootskitchen'target='_blank'>Desert Roots Kitchen</a> -- <b>Vegetarian cafe</b> with a daily changing menu of globally inspired cuisine made from local ingredients.</li>  
   	<li>The Munchies Cafe -- 
<b>Greek</b> eatery doling out gyros, subs, kebabs & salads in a casual setting with some sidewalk seating.</li> 
    </ul>

    <iframe src="https://www.google.com/maps/d/u/0/embed?mid=1E1sbe2BvR50OBgYTzHfsagp4CamrgiI1" width="640" height="480"></iframe>

	<p>The first three </p>

<h3>House of Tricks</h3>

<h3>P.F. Chang's</h3>

<h3>Pita Jungle - Tempe</h3>

American 

<h3>The Chuckbox</h3>

<h3>The Munchies Cafe</h3>
	<p> keywords: greek fastfood; lunch special </p>

<h3>Desert Roots Kitchen</h3>
	<p> Vegetarian cafe with a daily changing menu of globally inspired cuisine made from local ingredients. </p>




    </div>
 </div>
