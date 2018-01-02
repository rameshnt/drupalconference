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
    <li><a href="<?php echo base_path(); ?>conference/admin/rooms">Conference Room Details</a></li>
    <li><a class="active" href="<?php echo base_path(); ?>conference/admin/calender">Booking Calendar</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/reservations">Reservations</a></li>
  </ul>
</div>

<div class="cf-admin-calender-content">
  <div class="calender-wrap">
    <div class="calender-room">
      <div>
        <span>Select room:</span>
        <select name="" id="" class="cal-room-selection">
          <?php
            foreach(get_rooms() as $id=>$room){
              $s=(isset($_GET['selected_room']) && $_GET['selected_room']==$room)?'selected':'';
              echo '<option  name="'.$id.'"'. $s.'>'.$room.'</option>';
            }

          ?>
        </select>
        <div class="advance-booking-variable" id="<?php echo get_advance_booking_days_variable(); ?>"></div>
      </div>
      <div class="room-desc">
        <span>
          Description: 
          <?php

            if(isset($_GET['selected_room'])){
              $rname=$_GET['selected_room'];
              echo db_get_room_desc($_GET['selected_room']);
            }else{
              $rname=db_get_room_name();
              echo db_get_room_desc(); 
            }
          ?>
        </span>
      </div>
    </div>
    <div id="calendar"></div>
    <div id="booking-form">
      <div class="msg"></div>
      <div class="form">
        <div class="label">New Booking for <?=$rname;?></div>
        <input type="text" placeholder="Date" class="date-field" disabled>
        <div class="label">Timings</div>
        <div class="date">
          <span>Start time:</span>
          <span>
            <select name="" class="start-time" id="">
              <?php
                $st=get_starttimings();
                unset($st[999999]);
                foreach($st as $id=>$name){
                  echo "<option value='".$id."'>".$name."</option>";
                }
              ?>
            </select>
          </span>
        </div>
        <div class="date">
          <span>Duration (minutes):</span>
            <select name="" class="end-time" id="">
             <?php
                $st=db_get_advance_booking_hours();
                foreach(range(30, $st, 30) as $number){
                  $m=date('i',strtotime('+'.$number.' minutes'));
                  if($m>30)
                    $m=30;else $m=sprintf('%02d', 00);
                  $id=date('H'.$m.'00',strtotime('+'.$number.' minutes'));
                  echo "<option value='".$number."'>".$number."</option>";
                }
              ?>
            </select>
        </div>
        <div class="label">Meeting Description</div>
        <div class="desc">
          <textarea name="" id="" rows="3"></textarea>
        </div>
        <div class="form-ajax-submit">
          <button class="cancel">Cancel</button>
          <button class="save">Book</button> 
        </div>
      </div>
    </div>

    <div id="cancel-form">
      <div class="msg"></div>
      <div class="eid-variable"></div>
      <div class="id-variable"></div>
      <div class="form">
        <div class="label">Cancel Booking</div>
        <input type="date" placeholder="Date" class="date-field" disabled>
        <div class="label">Booked by: <span class="booked_by"></span></div>
        <div class="date">
          <span>Start time:</span>
          <span>
            <select name="" class="start-time" id="" disabled>
            </select>
          </span>
        </div>
        <div class="date">
          <span>Duration (minutes):</span> 
            <select name="" class="end-time" id="" disabled>
            </select>
        </div>
        <div class="label">Meeting Description</div>
        <div class="desc">
          <textarea name="" id="" rows="3" disabled></textarea>
        </div>
        <div class="form-ajax-submit">
          <button class="remove-booking">Cancel Booking</button>
        </div>
      </div>
    </div>

    <div id="error-alert">
      <div class="error-msg">
        <div class="msg-container"></div>
      </div>
    </div>
  </div>
</div>

<script>
  jQuery('.cal-room-selection').change(function(){
    var val=jQuery(this).val();
    window.location.assign(window.location.pathname+'?selected_room='+val);
  });
</script>