<!-- Portfolio -->

<script>
var slideIndex = 1;
var slideIndex1 = 1;
var slideIndex2 = 1;
showDivs(slideIndex);
showDivs1(slideIndex1);
showDivs2(slideIndex2);
function plusDivs(n) {
  showDivs(slideIndex += n);
}
function plusDivs1(n) {
  showDivs1(slideIndex1 += n);
}

function plusDivs2(n) {
    showDivs2(slideIndex2 += n);
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

function showDivs2(n) {
    var i;
    var x = document.getElementsByClassName("mySlides2");
    if (n > x.length) {slideIndex2 = 1}
    if (n < 1) {slideIndex2 = x.length}
    for (i = 0; i < x.length; i++) {
        x[i].style.display = "none";
    }
    x[slideIndex2-1].style.display = "block";
}



</script>

<h1>Venue</h1>

<ul class='arrows'>
<li><a href='#conference'>Conference Venue</a></li>
<li><a href='#workshops'>Workshops Venue</a></li>
<li><a href='#travelinfo'>Travel Information</a>
	<ul class='arrows'>
		<li>Conference Hotel Airport Shuttle Information</li>
		<li>Self-Driving Directions to Conference Hotel</li>
        <li>Taking Taxi/Uber/Lyft</li>
	</ul>
</li>
<li><a href='#foodinfo'>Restaurant information</a></li>
<li><a href='#banquetInfo'>KR banquet information</a></li>


</ul>



<p id='conference'></p>	
<br/><br/>
<h2>CONFERENCE VENUE</h2>

<p>KR 2018 main conference will be held at the:</p>

<p>
	<b>Tempe Mission Palms Hotel and Conference Center</b><br/>
	<a href='https://www.destinationhotels.com/tempe-mission-palms' target='_blank' title='Tempe Mission Palms Hotel'>Hotel Website <span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
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

<a style="cursor: pointer;"><img src="img/hotel_map.jpg" onclick="document.getElementById('modal01').style.display='block'" class="w3-hover-opacity" style="width:100%;height:100%"></a>

<div id="modal01" class="w3-modal w3-animate-zoom" onclick="this.style.display='none'">

<img class="w3-modal-content" src="img/hotel_map.jpg" style="width:100%;height:100%">
</div>


<p id='workshops'></p>	
<br/><br/>
<h2>WORKSHOPS VENUE</h2>


<p>KR 2018 workshops and tutorials session will be held at the:</p>

<p>
	<b>ASU Memorial Union </b><br/>
	<a href='https://eoss.asu.edu/mu' target='_blank' title='ASU Memorial Union'>Website<span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
    <a href='doc/mu_map.pdf' target='_blank' title='ASU Memorial Union Floor Map'>ASU MU Floor Map<span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
	301 E Orange St</br>
	Tempe, AZ 85281</br>
	United States</br>
	Phone: +1 (480) 965-5728

<div class='panel panel-warning'>
<div class='panel-heading' style='padding:10px; color:black;'>
On Sunday, only a few restaurants are open in Memorial Union. To find more restaurants, you can walk towards north from Memorial Union for about 8 minutes. After going across University Drive, you will find a lot of restaurants on College Ave and Mill Ave shown in the north-west corner of the <a href='#foodMap'>restaurant map</a>. All restaurants that are open on Sunday are colored in blue in the map, while the restaurants in grey are closed on Sunday but open on all other days (except for Chick-In, located next to Memorial Union, which is closed on Saturday).<br>Click <a href='#foodinfo'>here</a> for more restaurant information.
</div>
</div>
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

	<p>Tempe Mission Palms Hotel, where KR 2018 main conference will be held, is conveniently located near Arizona State University (ASU), and just 10 minutes' drive from Phoenix Sky Harbor International Airport and all major highways in and around Phoenix and Scottsdale. It is also located within walking distance to downtown Tempe's shopping, dining and entertainment district.</p>
	
	<p>The Memorial Union, considered the living room of the campus, will be holding KR 2018 workshops and tutorials sessions. It has a variety of activities and dining choices. Please visit <a href='https://eoss.asu.edu/mu'target='_blank'>MU website</a> for more information. </p>
    <iframe src="https://www.google.com/maps/d/u/0/embed?mid=17KtMkDK9ytAes9VLAvtSk5-XJfZnjuDI" width="640" height="480"></iframe>

<br>
<h2>Airport Shuttle Information</h2>
	<p>The conference hotel provides daily shuttle transportation to/from Sky Harbor International Airpot <b>from 5:30am to 10:00pm</b></p>
	
	<p><b>Arrival Directons from Terminal To Tempe Mission Palms</b><br/></p>
    <ul type="circle">
        <li>Pick up your luggage at the baggage claim first.</li>
        <li>Call Tempe Mission Palms Operator at 480-894-1400 using your personal cell phone or airport pay phone.</li>
        <li>Tell the operator what terminal you are located in.</li>
        <li>Follow the directions from the operator.</li>
        <li>Tempe Mission Palms vans are white and have the hotel logo on them.</li>
        <div class="w3-content w3-display-container">
             <img src="img/gal/hotel/shuttle.jpg" style="width:50%;height:50%">
        </div>
        <li>Shuttle should arrive within a few minutes. Time to hotel is about 10 minutes.</li>
    </ul>

	
	<p><b>Departure to the Airport From Tempe Mission Palms</b><br/>
    It is still recommended that individuals arrive at the airport 1 ½ to 2 hours prior to departure time. Please take that into consideration when deciding when to leave the hotel.<p/><p>The shuttle leaves on the hour and the half hour. Please be down in the front lobby near the Bell desk 10 minutes prior to shuttle departure. Let a Bellman know you are taking the next shuttle.
	<div class='panel panel-warning'>
	<div class='panel-heading' style='padding:10px; color:black;'>
	Guests will be taken to the airport on a space available basis. The conference hotel <b>DOES NOT</b> take reservations.
	</div>
	</div>
    </p>
	
	
<h2>Self-Driving Directions to Conference Hotel</h2>

	
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

<h2>Taking Taxi/Uber/Lyft</h2>
    <p><b>Taxi terminal access is as follows:</b></p>
    <ul class='arrows'>
        <li>Terminal 2: east outside door # 8</li>
        <li>Terminal 3: North curb, outside door # 7</li>
        <li>Terminal 4: Level 1, North curb, outside door # 7 and south curb, outside door #6</li>
    </ul>
    <p>The following taxis are contracted to pick up passengers at Phoenix Sky Harbor:</p>
    <ul class='arrows'>
    <li>AAA/Yellow Cab. <br/> Website: <a href="http://www.yellowcabaz.com/" target="_blank">http://www.yellowcabaz.com/</a><br/>Phone #: 480-888-8888</li>
    <li>Mayflower Cab. <br/> Phone #: 602-955-1355</li>
    <li>VIP TAXI. <br/> Website: <a href="http://viptaxi.com" target="_blank">http://viptaxi.com</a><br/>Phone #: 602-300-3000</li>
    </ul>
    <p>Please visit <a href="https://www.skyharbor.com/ParkingTransportation/TaxisAndShuttles#Taxis">here</a> for more taxi information</p>

    <p><b>Riding with Uber/Lyft</b></p>
    <p>Riding with Uber/Lyft is another affordable way of coming to Tempe. The average rate from Phoenix Sky Harbor airport to Tempe by riding Uber/Lyft will start from $14.00. Rate may vary depending on the exact destination you will be going. <br/>The following link can give an estimation: </p>
    <ul class='arrows'>
    <li>Uber Estimator: <a href="https://www.uber.com/fare-estimate/" target="_blank">https://www.uber.com/fare-estimate</a></li>
    <li>Lyft Estimator: <a href="https://www.lyft.com/fare-estimate" target="_blank">https://www.lyft.com/fare-estimate</a></li>

    </ul>






    </div>
 </div>





<p id='foodinfo'></p>	
<br/><br/>
<h2>RESTAURANT INFORMATION</h2>

<div>
    <div>

    <p> There are quite a lot of good restaurants around the venue. You can find a restaurant that suits your taste through <a href='https://www.yelp.com/'target='_blank'>Yelp</a>.</p>

    <p> For your convenience, here we list some restaurants that are popular. Most of them include vegetarian meal on their menus. Their locations are also pinned at the end of this page. </p>
	
	<!-- <p><b>Restaurants near Memorial Union:</b></p>-->
    <ul class='arrows'>
	<li><a href='https://eoss.asu.edu/mu/'target='_blank'>Memorial Union</a> -- Sun Devil Dining offers a wide range of food venues in the Memorial Union including: <b>Engrained Cafe</b>, <b><font color="red">Pitchforks</font></b>, <b><font color="red">Burger King</font></b>, <b>Chick-fil-A</b>, <b>Einstein Bros. Bagels</b>, <b>Devils' Greens</b>, <b>Jamba Juice</b>, <b>AFC Sushi</b>, <b><font color="red">Qdoba</font></b>, <b>Pei Wei</b>, <b><font color="red">Subway</font></b>, <b><font color="red">Starbucks</font></b>, <b>Rosie's at the MU</b> and the <b><font color="red">MU P.O.D convenience market</font></b>. The location of each food venue can be found in <a href='https://eoss.asu.edu/sites/default/files/MU-buildingMap.pdf'target='_blank'>this map</a>. <b>(Only the restaurants in <font color="red">RED</font> are open on weekends. Please check <a href='https://sundevildining.asu.edu/hours-locations/tempe'target='_blank'>this website</a> for their opening hours.)</b></li>  
	
	<li><a href='https://www.destinationhotels.com/tempe-mission-palms/dining'target='_blank'>Tempe Mission Palms Hotel Dining</a> -- serving several Tempe restaurants & bar options, including Mission Grille, Harry's Place Lounge, and poolside Cabana Bar & Grill. </li> 
 	
   	<li><a href='https://chickinaz.com/'target='_blank'>Chick-In</a> -- Dining establishment featuring sandwiches, platters, chicken, salads, fries, drinks & more... <b><font color="red">(Saturday and Sunday Closed)</font></b> </li> 
    <!-- 
    </ul>

    <p><b>Restaurants near downtown of Tempe:</b></p>
	<ul class='arrows'>
	-->
	
	<li><a href='https://www.houseoftricks.com/'target='_blank'>House of Tricks</a> -- Cozy spot serving <b>New American fare</b> in a 1920s bungalow with a patio featuring a fire pit & bar. <b><font color="red">(Sunday Closed)</font></b> </li>  
 	<li><a href='https://www.pfchangs.com/locations/us/az/tempe/740-s-mill-ave/2800-tempe.html?utm_source=YextMain&utm_medium=Directory'target='_blank'>P.F. Chang's</a> -- Family-friendly chain offering creative takes on <b>Chinese fare</b> in a striking space.</li>  
   	<li><a href='http://www.pitajungle.com/locations/tempe-mill/'target='_blank'>Pita Jungle</a> -- Chain for health-oriented <b>Mediterranean dishes</b>, including lots of veggie options, in sleek environs.</li> 

	<li><a href='http://www.thechuckbox.com/Menu.html'target='_blank'>The Chuckbox</a> -- Basic, <b><font color="red">cash-only</font></b> <b>burger</b> joint turning out <b>charcoal-broiled patties</b> plus beer & wine.</li>  
 	<li><a href='https://www.mkt.com/desertrootskitchen'target='_blank'>Desert Roots Kitchen</a> -- <b>Vegetarian cafe</b> with a daily changing menu of globally inspired cuisine made from local ingredients. <b><font color="red">(Sunday Closed)</font></b></li>
 	<li><a href='https://www.donerdudes.com/'target='_blank'>Doner Dudes</a> -- <b>Mediterranean</b> street food. Outdoor seating. </li> 
 	<li><a href='http://www.thaibasilasu.com/'target='_blank'>Thai Basil</a> -- An extensive selection of standard <b>Thai dishes</b> draws diners to this casual eatery with beer & wine.  </li>
 	<li><a href='https://thehalalguys.com/'target='_blank'>The Halal Guys</a> -- Offshoot of a famed food cart offering gyros, chicken & rice platters & other <b>Middle Eastern eats</b>.  </li>
 	<li><a href='https://evenstevens.com/'target='_blank'>Even Stevens Sandwiches</a> -- A craft-casual restaurant serving great sandwiches in relaxed environment.  </li>
 	<li><a href='https://www.ahipokibowl.com/'target='_blank'>Ahipoki Bowl</a> -- A <b>California coastal kitchen</b> serving the freshest ahi poke seafood with the only the best ingredients and spices inspired by the diversity of people and culture from local California communities.  </li>
 	<li><a href='http://vinetavern.com/rural-apache/'target='_blank'>Vine Tavern & Eatery</a> -- Hopping hangout serving <b>American classics in a rustic setup</b> with sports on TVs, billiards & a patio.</li>
 	<li><a href='https://www.dominos.com/en/?utm_source=google&utm_medium=loclist&utm_campaign=localmaps'target='_blank'>Domino's Pizza</a> -- Delivery/carryout chain offering a wide range of pizza, plus chicken & other sides.</li>
 	<li><a href='https://www.jimmyjohns.com/'target='_blank'>Jimmy John's</a> -- Counter-serve chain specializing in sub & club sandwiches, plus signature potato chips.</li>
 	<li><a href='https://www.chick-fil-a.com/Locations/AZ/University-at-Mill'target='_blank'>Chick-fil-A</a> -- Fast-food chain serving chicken sandwiches, strips & nuggets along with salads & sides. <b><font color="red">(Sunday Closed)</font></b></li>
 	<li><a href='http://www.caseymoores.com/'target='_blank'>Casey Moore's Oyster House</a> -- <b>Irish</b> haunt serving seafood, pub grub, draft beer & whiskey in a cozy, memorabilia-filled setting.</li>
 	<li><a href='http://www.firstwatch.com/'target='_blank'>First Watch</a> -- Cafe chain for health-minded breakfast, brunch & lunch options such as omelets, salads & soups.</li>
 	<li><a href='https://www.blazepizza.com/'target='_blank'>Blaze Pizza</a> -- Hip counter-serve pizzeria dishing up crispy, thin-crust pies made with creative toppings & sauces.</li>
 	<li><a href='https://mellowmushroom.com/location/az-tempe-740-s-mill-ave-85281/'target='_blank'>Mellow Mushroom</a> -- Funky, art-filled chain pizzeria featuring craft beer, calzones & creative stone-baked pizzas.</li>
 	<li><a href='http://www.laboccapizzeria.com/'target='_blank'>La Bocca Urban Pizzeria + Wine Bar</a> -- Trendy spot serving locally sourced pasta, pizza & small plates plus dozens of wines by the glass.</li>
   	<li><a href='https://www.yelp.com/biz/the-munchies-cafe-tempe'target='_blank'>The Munchies Cafe</a> -- <b>Greek</b> eatery doling out gyros, subs, kebabs & salads in a casual setting with some sidewalk seating.</li>
   	<li><a href='https://www.chipotle.com/'target='_blank'>Chipotle Mexican Grill</a> -- Fast-food chain offering <b>Mexican fare</b>, including design-your-own burritos, tacos & bowls.</li>
   	<li><a href='https://originalchopshop.com/'target='_blank'>Original ChopShop</a> -- Cafe with outdoor seating, serving health-conscious juices, salads & sandwiches, plus beer & wine.</li>
   	<li><a href='https://www.pandaexpress.com/'target='_blank'>Panda Express</a> -- Fast-food chain for <b>Chinese</b> standards, including some health-conscious options.</li>
   	<li><a href='https://snoozeeatery.com/locations/tempe/'target='_blank'>Snooze an A.M. Eatery</a> -- Vibrant, retro chain serving a seasonal menu of creative breakfast & lunch fare, plus cocktails.</li>
   	<li><a href='https://www.postinowinecafe.com/'target='_blank'>Postino Annex</a> -- Wines and fresh, colorful bruschetta boards built for sharing.</li>
   	<li><a href='http://paradisehawaiinbbq.letseat.at/'target='_blank'>Paradise Hawaiian Barbeque</a> -- <b>Korean</b> barbeque. Island staples like spam musubi & loco moco served in a simple grill with flat-screen TVs. <b><font color="red">(Sunday Closed)</font></b></li>
   	

    </ul>
    <p id='foodMap'></p>
    <iframe src="https://www.google.com/maps/d/u/0/embed?mid=1E1sbe2BvR50OBgYTzHfsagp4CamrgiI1" width="640" height="480"></iframe>



    </div>
 </div>



<p id='banquetInfo'></p>
<br/><br/>
<h2>KR BANQUET VENUE</h2>

<br>
    <p>The Banquet for KR 2018 Main Conference will be held at the:</p>

    <p>
    <b>Desert Foothills Event Center</b><br/>
    <a href='http://www.desertfoothills.com' target='_blank' title='Desert Foothills Event Center'>Banquet Venue Website <span class="glyphicon glyphicon-link" aria-hidden="true"></span></a><br/>
    <div class='panel panel-warning'>
    <div class='panel-heading' style='padding:10px; color:black;'>
    Here will be shuttle running from conference hotel to KR2018 banquet location. In case you want to self-drive to the banquet location, please refer to following information:

    <p><b>Direction from the 101 freeway:</b></p>
    <ul type="circle">
    <li>The banquet venue does not have physical address, it's located in zip code 85255</li>
    <li>Exit Scottsdale Rd – head NORTH</li>
    <li>Approx 6 miles - Turn Left (WEST) on Jomax Rd (light)</li>
    <li>Look for the coyote – just after 70th Place on your LEFT</li>
    <div class="w3-content w3-display-container">
        <img src="img/gal/banquet/b_coyote.jpg" style="width:30%;height:30%">
    </div>
    <li>Turn left down dirt road and make your way to the barn.</li>
    <li>Please dirve slow</li>
    </ul>
    </div>
    </div>
    <a style="cursor: pointer;"><img src="img/gal/banquet/b_map.jpg" onclick="document.getElementById('modal02').style.display='block'" class="w3-hover-opacity" style="width:50%;height:50%"></a>
    <div id="modal02" class="w3-modal w3-animate-zoom" onclick="this.style.display='none'">

    <img class="w3-modal-content" src="img/gal/banquet/b_map.jpg">
    </div>
    </p>
    <br><br><p>The event venue consists of three areas: The Barn, The Cookout, and the West Lot. </p>
    <p>The Barn:</p><p>Adjacent to the outdoor cookout is our 10,000 square foot barn. This barn was built out of old telephone poles! With high ceilings, open beams and rafters, its rustic charm comes to life with strung twinkle lights throughout. The barn is L shaped, and in the corner of the L, we can have dance floor and riser for a band  The barn also has a 2500 square foot Patio – The Sunset Patio. As it faces west and we can have the best view of the sun setting over the desert. This area is great for just about everything: additional dining, specialty entertainment, lounge furniture, receptions, or enjoying a show in the west lot.</p>
    <p>The Cookout:</p><p>This is one of the outdoor spaces. Located at the entrance of the venue, the cookout consists of a nice wide open area surrounded by natural desert terrain. You will find four large in-ground fire pits, a dance floor, and a band-shell. This space is a great spot for an intimate group of 50 around one of the campfires, or a reception before moving into the barn.</p>
    <p>The West Lots:</p><p>This area is to the west of the barn. It is a wide open area surrounding by desert terrain. This lot is available to use if you have specialty entertainment. Big productions with large stages for top name entertainment are put in this lot.</p>



<div class="w3-content w3-display-container">
<img class="mySlides2" src="img/gal/banquet/1.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/2.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/3.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/4.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/5.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/6.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/7.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/8.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/9.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/10.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/11.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/12.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/13.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/14.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/15.jpg" style="width:100%">
<img class="mySlides2" src="img/gal/banquet/16.jpg" style="width:100%">
<button class="w3-button w3-black w3-display-left" onclick="plusDivs2(-1)">&#10094;</button>
<button class="w3-button w3-black w3-display-right" onclick="plusDivs2(1)">&#10095;</button>
</div>
</br>



<div style="text-align:left;"><h2>We are looking forward to meeting you in Arizona!</h2> </div><br><br><br>


<div style="text-align:right;"><i>The of texts and Images are adapted from Banquet venue webpage.</i></div>

