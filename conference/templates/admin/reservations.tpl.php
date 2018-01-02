<?php

global $user;
if (!in_array('administrator', $user->roles)) {
	drupal_set_message("You cannot access Admin functionality", 'error');	
	drupal_goto('/', array());	
}

drupal_add_library('system', 'ui.datepicker');
drupal_add_js("(function ($) { $('.datepicker').datepicker({dateFormat: 'yy-mm-dd'}); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));

?>
<div class="cf-admin-menu">
  <ul>
    <li><a href="<?php echo base_path(); ?>conference/admin/settings">Settings</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/configurations">Configurations</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/rooms">Conference Room Details</a></li>
    <li><a href="<?php echo base_path(); ?>conference/admin/calender">Booking Calendar</a></li>
    <li><a class="active" href="<?php echo base_path(); ?>conference/admin/reservations">Reservations</a></li>
  </ul>
</div>
<?php
if(isset($_POST['filterdate']))
  $exportdate=$_POST['filterdate'];
  elseif(isset($_POST['filterfdate']))
  $exportdate=$_POST['filterfdate'].'|'.$_POST['filtertdate'];
  elseif(isset($_POST['filterroomsubmit']))
  $exportdate=$_POST['filterroom'].'-nocontent';
  else
  $exportdate='all';

?>

<div class="cf-admin-reservations-content">
  <div class="reservations-wrap">
    <div class="cf-admin-menu">
    	<ul>
    	     <li><a href="<?php echo base_path(); ?>conference/admin/reservations/export?date=<?php echo $exportdate; ?>">Export to Excel</a></li>
           <li>
             <form action="" method="post">
               <input type="text" name="filterdate" placeholder="select date" class="datepicker"/><input type="submit" value="search" name="filterdatesubmit" class="button">
             </form>
           </li>
           <li>
             <form action="" method="post">
               <input type="text" name="filterfdate" placeholder="from date" class="datepicker" required/>
               <input type="text" name="filtertdate" placeholder="to date" class="datepicker" required/>
               <input type="submit" value="search" name="filterftdatesubmit" class="button">
             </form>
           </li>
    	</ul>
      <ul>
        <li>
          <form action="" method="post">
            <select name="filterroom" id="" class="cal-room-selection" style="display:inline-block;width:281px">
              <?php
                foreach(get_rooms() as $id=>$room){
                  $s=(isset($_GET['selected_room']) && $_GET['selected_room']==$room)?'selected':'';
                  echo '<option  value="'.$id.'"'. $s.'>'.$room.'</option>';
                }
              ?>
            </select>
            <input type="submit" value="search" name="filterroomsubmit" class="button" style="display:inline-block;">
          </form>
        </li>
      </ul>
    </div>

<?php
$rooms=get_rooms();
          $st=get_starttimings();
          $et=get_endtimings();
          if(isset($_POST['filterdatesubmit'])){
            $reserved=get_reserved_details_bydate($_POST['filterdate']);
          }
          elseif(isset($_POST['filterftdatesubmit'])){
            $reserved=get_reserved_details_btwdate($_POST['filterfdate'],$_POST['filtertdate']);
          }
          elseif(isset($_POST['filterroomsubmit'])){
            $reserved=get_reserved_details_byroom($_POST['filterroom']);
          }else{
            $reserved=get_reserved_details();
          }
          if(empty($reserved)){
            echo '<div style="padding: 18px;display: block;width: 100%;font: normal small &quot;Lucida Grande&quot;, Verdana, sans-serif;">No booking found.</div>';
          }else{

?>
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
            foreach($reserved as $items){
              $uid=user_load($items->userid);
              echo "<tr><td>".date('d F Y',strtotime($items->date))."</td><td>".$uid->name."</td> <td>".$rooms[$items->roomid]."</td><td>".$st[$items->starttime]."</td><td>".$et[$items->endtime]."</td></tr>";
            }
        ?> 

      </tbody>
    </table>


<?php
          }

?>
  </div>


</div>