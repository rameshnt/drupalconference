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
    <li><a class="active" href="<?php echo base_path(); ?>conference/admin/configurations">Configurations</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/rooms">Conference Room Details</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/calender">Booking Calendar</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/reservations">Reservations</a></li>
  </ul>
</div>

<div class="cf-admin-configurations-content">
  <div class="cf-admin-menu">
    <ul>
      <li><a class="h<?php if(!isset($_GET['month-days']))echo ' active';?>" href="?">Hours</a></li>
      <li><a class="mh<?php if(isset($_GET['month-days']))echo ' active';?>" href="?month-days">Monthly Hours</a></li>
      <li class="month">
        <select name="" id="" class="month-config">
          <?php
          $selected_month='';
          if(isset($_GET['month']))
            $selected_month=$_GET['month'];
          
          $settings=db_get_settings();
          $max_reservation=$settings['max-reservation'];
          $total_months=($max_reservation/30);
          for($i=0;$i<=$total_months;$i++){
            $month=date('F',strtotime('+'.$i.' month',strtotime(date("F") . "1")));
            $year=date('Y',strtotime('+'.$i.' month'));
            $val=$month.'-'.$year;
              $selected='';
            if($selected_month==$month)
              $selected='selected';
            echo '<option value="'.$val.'" '.$selected.'>'.$month.'</option>';
          }
          ?>
        </select>
      </li>
    </ul>
  </div>

  <?php

  if(isset($_GET['month-days'])){
    echo '<div class="configurations-wrap monthly-hours">';
      echo $configurations_month_form;
    echo '</div>';
  }else{
      echo '<div class="configurations-wrap hours">';
        echo $configurations_week_form;
      echo '</div>';
  }

  ?>
  <script>
    jQuery('.month-config').change(function(){
      var val=jQuery(this).val();
      val=val.split('-');
      window.location.assign(window.location.pathname+'?month-days&month='+val[0]+'&year='+val[1]);
    });
  </script>