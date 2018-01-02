<?php

global $user;

if (!in_array('Conference', $user->roles)) {
	drupal_set_message("You do not have conference role: Please contact administrator", 'error');	
	drupal_goto('conference', array());	
}
/*
if(!$user->uid){
	drupal_goto('/', array());
}
*/

?>

<div class="cf-user-menu">
  <ul>
    <li><a href="<?php echo base_path(); ?>conference/user/calender">Booking Calendar</a></li>
    <li><a href="<?php echo base_path(); ?>conference/user/reservations">My Reservations</a></li>
    <li><a class="active" href="<?php echo base_path(); ?>conference/user/rooms">Conference Room Details</a></li>
  </ul>
</div>

<div class="cf-user-rooms-content"> 
  <div class="rooms-wrap">
    <ul>
      <?php
        $rooms=get_rooms();
        foreach(get_rooms_details() as $items){
          echo '<li><div><span class="title">Conference Room:</span> <span class="text">'.$items->name.'</span></div> <div><span class="title">Seating Capacity:</span> <span class="text">'.$items->capacity.' people</span></div><div><span class="title">Description:</span> <span class="text">'.$items->description.'</span></div></li>';
        }

          
      ?>
      
    </ul>
  </div>
</div>