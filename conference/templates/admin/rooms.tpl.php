<?php

global $user;

if (!in_array('administrator', $user->roles)) {
	drupal_set_message("You cannot access Admin functionality", 'error');	
	drupal_goto('/', array());	
}
/*
if(!$user->uid){
	drupal_goto('/', array());
}
*/

?>
<div class="cf-admin-menu">
  <ul>
    <li><a href="<?php echo base_path(); ?>conference/admin/settings">Settings</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/configurations">Configurations</a></li>
    <li><a class="active" href="<?php echo base_path(); ?>conference/admin/rooms">Conference Room Details</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/calender">Booking Calendar</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/reservations">Reservations</a></li>
  </ul>
</div>

<div class="cf-admin-rooms-content">
  <?php
    if(isset($_GET['confirm']) && $_GET['confirm']==0){
      drupal_set_message('Deleting the rooms will remove all the booking in the room. Proceed? <a href="'.base_path().'conference/admin/rooms?confirm=1&rid='.$_GET['rid'].'&rname='.$_GET['rname'].'">yes</a>','warning');
    }
      elseif(isset($_GET['confirm']) && $_GET['confirm']==1){
        conference_delete_room($_GET['rid'],$_GET['rname']);
      }
  ?>
  <div class="rooms-wrap">
    <?php echo $rooms_form; ?>
  </div>
	<ul>
      <?php
        $rooms=get_rooms();
        foreach(get_rooms_details() as $items){
          echo '<li><div><span class="title">Conference Room:</span> <span class="text">'.$items->name.'</span></div> <div><span class="title">Seating Capacity:</span> <span class="text">'.$items->capacity.' people</span></div><div><span class="title">Description:</span> <span class="text">'.$items->description.'</span></div><div class="edit"><a href="'.base_path().'conference/admin/rooms?confirm=0&rid='.$items->id.'&rname='.$items->name.'" class="btn delete">Delete</a></div></li>';
        }

          
      ?>
      
    </ul>
</div>