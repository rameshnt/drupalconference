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
    <li><a class="active" href="<?php echo base_path(); ?>conference/admin/settings">Settings</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/configurations">Configurations</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/rooms">Conference Room Details</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/calender">Booking Calendar</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/reservations">Reservations</a></li>
  </ul>
</div>

<div class="cf-admin-settings-content">
  <div class="settings-wrap">
    <?php echo $settings_form; ?>
  </div>
</div>