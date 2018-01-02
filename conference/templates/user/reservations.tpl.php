<?php

global $user;

if (!in_array('Conference', $user->roles)) {
	drupal_set_message("You do not have conference role: Please contact administrator", 'error');	
	drupal_goto('conference', array());	
}
/*
if (!in_array('conference', $user->roles)) {
	drupal_goto('/', array());	
}


if(!$user->uid){
	drupal_goto('/', array());

}
*/

?>

<div class="cf-user-menu">
  <ul>
    <li><a href="<?php echo base_path(); ?>conference/user/calender">Booking Calendar</a></li>
    <li><a class="active" href="<?php echo base_path(); ?>conference/user/reservations">My Reservations</a></li>
    <li><a href="<?php echo base_path(); ?>conference/user/rooms">Conference Room Details</a></li>
  </ul>
</div>

<div class="cf-user-reservations-content">
  <div class="reservations-wrap">
    <table class="reservations">
      <thead>
        <tr>
          <td>Date </td>
          <td>Booked by </td>
          <td>Conference Room </td>
          <td>Start time</td>
          <td>End time</td>
        </tr>
      </thead>
      <tbody>
        <?php
          $rooms=get_rooms();
          $st=get_starttimings();
          $et=get_endtimings();
          $q=get_user_reserved_details($user->uid);
          if(!$q){
            echo "<tr><td>You have not yet booked any room.</td><td></td><td></td><td></td><td></td></tr>";
          }
          else{
            foreach(get_user_reserved_details($user->uid) as $items){
              echo "<tr><td>".date('d F Y',strtotime($items->date))."</td><td>".$user->name."</td> <td>".$rooms[$items->roomid]."</td><td>".$st[$items->starttime]."</td><td>".$et[$items->endtime]."</td></tr>";
            }
          }
          
        ?>
      </tbody>
    </table>
  </div>


</div>