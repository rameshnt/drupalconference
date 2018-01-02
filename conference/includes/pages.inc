<?php

global $user;

function __conference_calender_page(){
  return theme('conference_calender_page');
}
/*
  USER
*/

  function __conference_user_calender_page(){
    return theme('conference_user_calender_page');
  }
  function __conference_user_reservations_page(){
    return theme('conference_user_reservations_page');
  }
  function __conference_user_rooms_page(){
    return theme('conference_user_rooms_page');
  }

/*
  ADMIN
*/

function __conference_admin_settings_page(){
  $form_v=drupal_get_form('__conference_form_admin_settings');
  $form=drupal_render($form_v);
  return theme('conference_admin_settings_page',array(
    'settings_form'=>$form
    ));
}

function __conference_admin_configurations_page(){
  $form_v=drupal_get_form('__conference_form_admin_configurations_week');
  $form_m=drupal_get_form('__conference_form_admin_configurations_month');
      $form=drupal_render($form_v);
      $mform=drupal_render($form_m);
      return theme('conference_admin_configurations_page',array(
        'configurations_week_form'=>$form,
        'configurations_month_form'=>$mform,
        ));
}

function __conference_admin_rooms_page(){
  $form_v=drupal_get_form('__conference_form_admin_rooms');
    $form=drupal_render($form_v);
    return theme('conference_admin_rooms_page',array(
      'rooms_form'=>$form
      ));
}

function __conference_admin_calender_page(){
  return theme('conference_admin_calender_page');
}

function __conference_admin_reservations_page(){
	return theme('conference_admin_reservations_page');
}

/*AJAX CALSS*/

function __conference_ajax_calls_page(){
  global $user;
  $args=drupal_get_query_parameters();
  if(!isset($args['dated'])){
    $args['msg']='Please select the date';
  $args['type']='error';
  }

  if(___is_room_booked($args['room'],$args['st'],$args['et'],$args['date'])){
    drupal_set_message('Room '.$rooms[variable_get('user_selected_room')].' booked','error');
    $args['msg']='Room already booked';
    $args['type']='error';
    return drupal_json_encode($args);
  }

  $q=___db_insert_booking($user->uid,$args['room'],$args['st'],$args['et'],$args['date'],$args['desc']);
  if($q){
    $args['msg']='Reserved';
    $args['type']='success';
    $args['username']=$user->name;
  }
  else{
    $args['msg']='Please try again after some time';
    $args['type']='error';
  }
  return drupal_json_encode($args);
    
}

function __conference_ajax_getreserved_page(){
  return get_reserved_details_ajax();
}


function ___do_reserve($details){
    global $user;
    $st=$details['st'];
    $et=$details['et'];
    $details=$details['desc'];
    $roomid=$details['room'];
    $date=$details['date'];

    if(___is_room_booked($roomid,$st,$et,$date)){
      drupal_set_message('Room '.$rooms[variable_get('user_selected_room')].' booked','error');
      return;
    }
    $q=db_add_booking($user->uid,$roomid,$st,$et,$date,$details);
    if($q)
    drupal_set_message('Room '.$rooms[variable_get('user_selected_room')].' booked');
}

function ___db_insert_booking($uid,$roomid,$st,$et,$date,$details){
  $fs=db_insert('conference_booking')
    ->fields(array(
      'userid'=>$uid,
      'roomid'=>$roomid,
      'starttime'=>$st,
      'endtime'=>$et,
      'date'=>$date,
      'details'=>$details,
    ))->execute();
  return $fs;
}

function ___is_room_booked($roomid,$st,$et,$date){
  $q = db_query("SELECT * FROM conference_booking WHERE starttime='".$st."' AND endtime='".$et."' AND date='".$date."' AND roomid=".$roomid);
  return $q->fetch();
}


function get_advance_booking_days_variable(){
  $q = db_query("SELECT * FROM conference_settings WHERE name='max-reservation'");
  $qq=$q->fetch();
  return $qq->value;
}


/* main ajax calls*/


function __get_advance_booking_days_page(){
  $q = db_query("SELECT * FROM conference_settings WHERE name='advance-booking'");
  $qq=$q->fetch();
  return json_encode(array('days'=>$qq->value));
}

function __get_advance_booking_v(){
  $q = db_query("SELECT * FROM conference_settings WHERE name='advance-booking'");
  $qq=$q->fetch();
  return $qq->value;
}

function __check_month_booking_timings($date,$st,$et){
  $q = db_query("SELECT * FROM conference_month_hours WHERE month='".$date."'");
  $qq=$q->fetch();
  if($qq){
    if($st>=$qq->starttime && $et<=$qq->endtime){
      return true;
    }
  }
    return __check_week_booking_timings($date,$st,$et);
}

function __check_week_booking_timings($date,$st,$et){
  $date=date('l',strtotime($date));
  $q = db_query("SELECT * FROM conference_week_hours WHERE weekday='".$date."'");
  $qq=$q->fetch();
  if($qq){
    if($st>=$qq->starttime && $et<=$qq->endtime)
      return true;
  }
  return false;
}

function __check_month_booking_holiday_timings($date,$st,$et){
  $q = db_query("SELECT * FROM conference_month_hours WHERE month='".$date."'");
  $qq=$q->fetch();
  if($qq){
    if($qq->starttime==999999 || $qq->endtime==999999){
      return true;
    }
  }
    return __check_week_booking_holiday_timings($date,$st,$et);
}

function __check_week_booking_holiday_timings($date,$st,$et){
  $date=date('l',strtotime($date));
  $q = db_query("SELECT * FROM conference_week_hours WHERE weekday='".$date."'");
  $qq=$q->fetch();
  if($qq){
    if($qq->starttime==999999 && $qq->endtime==999999)
      return true;
  }
  return false;
}

function __get_inbetween_days($date){
	$startTimeStamp = time();
	$endTimeStamp = strtotime($date);

	$timeDiff = abs($endTimeStamp - $startTimeStamp);

	$numberDays = $timeDiff/86400;

	return  intval($numberDays);

}
function __format_endtime($st,$et){
    $mktime=mktime($st[0].$st[1], $st[2].$st[3], $st[4].$st[5]);
    $m=date('i',strtotime('+'.$et.' minutes',$mktime));
    if($m<30)
      $m=sprintf('%02d', 00);else $m=30;
    return date('H'.$m.'00',strtotime('+'.$et.' minutes',$mktime));
}

function __get_times($st,$et){
  $et_count=$et/30;
  $times=array();
  foreach(range(0,$et_count) as $val){
    $times[]=__format_endtime($st,$val*30);
  }
  $remove_last=array_pop($times);
  return $times;
}

function __get_timestamp($st,$date){
  $hr=$st[0].$st[1];
  $min=$st[2].$st[3];
  $sec=$st[4].$st[5];
  $date=explode('-', $date);
  $yr=$date[0];
  $mn=$date[1];
  $day=$date[2];
  return mktime($hr,$min,$sec,$mn,$day,$yr);
}


function __add_ajax_event_page(){

  global $user;
  $userid=$user->uid;
  $args=drupal_get_query_parameters();
  $args['username']=$user->name;
  $args['times']=__get_times($args['st'],$args['et']);
  $args['et']=__format_endtime($args['st'],$args['et']);
  $room=get_rooms();
  $room=array_flip($room);

  /*
  * 1.0.0. check user logedin
  */
  if(!$user->uid){
    $args['msg']='Please login to book your room';
    $args['type']='error';
    $args['login']=true;
    return drupal_json_encode($args);
  }

  /*
  * 1.0.1. default description text
  */
  if($args['desc']==''){
    $args['msg']='Please provide description.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 1.0.2. check days span
  */
  if( __get_inbetween_days($args['date'])>360){
    $args['msg']='Please select valid date.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 1.0.3. check positive timings
  */
  if((($args['et']-$args['st'])>=3000)==false){
    $args['msg']='Please select proper timing.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 1.0.4. check future date
  */
  if(floor((time() -strtotime($args['date']))/(60*60*24))>0){
    $args['msg']='Please select future date.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 1.0.5. check future time
  */
  if(time()>__get_timestamp($args['st'],$args['date'])){
    $args['msg']='Please select future time.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 1. check holiday
  */
  if(__check_month_booking_holiday_timings($args['date'],$args['st'],$args['et'])==TRUE){
    $args['msg']='Selected day is a holiday. Please select an other date.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 2. check timings
  */
  if(__check_month_booking_timings($args['date'],$args['st'],$args['et'])==FALSE){
    $args['msg']='Selected timings are not allowed on this date. Please select other timings.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 3. check another room booked
  */
  if(is_another_room_booked($room[$args['roomid']],$userid,$args['times'],$args['date'])){
    $args['msg']='You have booked another room at the same timing. Please select another timings options.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 3.1. check room booked
  */
  if(is_room_booked($room[$args['roomid']],$args['times'],$args['date'])){
    $args['msg']='Room already booked for the selected time. Please select another timings options.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 3.2. check middle time room booked
  */

  if(is_middletime_room_booked($args['st'],$args['date'],$room[$args['roomid']])){
    $args['msg']='Room already booked middle of the selected time. Please select another timings options.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 3.2. check middle time room booked
  */

  if(is_another_room_booked_middletime($args['st'],$args['date'],$room[$args['roomid']],$userid)){
    $args['msg']='You have already booked another room middle of the selected time. Please select another timings options.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 3.3. check time reserved
  */

  if(is_another_room_booked_middletime($args['st'],$args['date'],$room[$args['roomid']],$userid)){
    $args['msg']='You have already booked another room middle of the selected time. Please select another timings options.';
    $args['type']='error';
    return drupal_json_encode($args);
  }
  /*
  * 4. check maximum booking hours
  */
  $to_time = strtotime($args['et']);
  $from_time = strtotime($args['st']);
  $diff=round(abs($to_time - $from_time) / 60,2);
  if($diff>__get_advance_booking_v()){
    $args['msg']='You have selected maximum hours. Please shorten the timings.';
    $args['type']='error';
    return drupal_json_encode($args);
  }

  /*
  * 5. add booking event
  */
  $reserve=db_add_booking($user->uid,$room[$args['roomid']],$args['st'],$args['et'],$args['date'],$args['desc']);
  if($reserve){
    $args['msg']='Room has been booked.';
    $args['type']='success';
    return drupal_json_encode($args);
  }

}

function __remove_ajax_event_page(){
  $args=drupal_get_query_parameters();
  if(!isset($args['eid'])){
    $args['msg']='No sufficient details.';
    $args['type']='error';
    return drupal_json_encode($args);
  }

  $q=db_delete_booked_room($args['eid']);
  if($q){
    $args['msg']='Booking Removed from the record.';
    $args['type']='success';
    return drupal_json_encode($args);
  }

  $args['msg']='Something went wrong. Please try again after some time.';
  $args['type']='error';
  return drupal_json_encode($args);

}

function __remove_user_ajax_event_page(){
  global $user;
  $args=drupal_get_query_parameters();
  if(!isset($args['eid'])){
    $args['msg']='No sufficient details.';
    $args['type']='error';
    return drupal_json_encode($args);
  }


  $q=db_delete_user_booked_room($user->uid,$args['eid']);
  if($q){
    $args['msg']='Booking Removed from the record.';
    $args['type']='success';
    return drupal_json_encode($args);
  }else{
    $args['msg']='You can only delete your own booking.';
    $args['type']='error';
    return drupal_json_encode($args);
  }

  $args['msg']='Something went wrong. Please try again after some time.';
  $args['type']='error';
  return drupal_json_encode($args);

}


function __conference_admin_reservations_export(){
  if(count(explode('|', $_GET['date']))==2){
    $date=explode('|', $_GET['date']);
    $db_record=get_reserved_details_btwdate($date[0],$date[1]);
  }
  elseif(count(explode('-', $_GET['date']))==2){
    $date=explode('-', $_GET['date']);
    $db_record=get_reserved_details_byroom($date[0]);
  }
  elseif($_GET['date']=='all')
    $db_record=get_reserved_details();
  else
    $db_record=get_reserved_details_bydate($_GET['date']);

	require_once(libraries_get_path('PHPExcel').'/Classes/PHPExcel.php');
	
        $objPHPExcel = new PHPExcel();
   
   	$objPHPExcel->setActiveSheetIndex(0);
   	$objPHPExcel->getActiveSheet()->SetCellValue('A1', 'Date');
   	$objPHPExcel->getActiveSheet()->SetCellValue('B1', 'Booked by');
   	$objPHPExcel->getActiveSheet()->SetCellValue('C1', 'Conference room');
   	$objPHPExcel->getActiveSheet()->SetCellValue('D1', 'Start time');
   	$objPHPExcel->getActiveSheet()->SetCellValue('E1', 'End time');
   	
   	function apply_style($objPHPExcel,$cell){
   		$objPHPExcel->getActiveSheet()->getStyle($cell)->applyFromArray(
		    array(
		        'fill' => array(
		            'type' => PHPExcel_Style_Fill::FILL_SOLID,
		            'color' => array('rgb' => '0779bf')
		        ),
		        'font'  => array(
			        'color' => array('rgb' => 'ffffff')
			    )
		    )
		);
   	}
   	apply_style($objPHPExcel,'A1');
   	apply_style($objPHPExcel,'B1');
   	apply_style($objPHPExcel,'C1');
   	apply_style($objPHPExcel,'D1');
   	apply_style($objPHPExcel,'E1');

	$i=2;
	foreach($db_record as $k=>$v){

		$users=user_load($v->userid);
		$rooms=get_rooms();
		$st=get_starttimings();
		$et=get_endtimings();

		$date=$v->date;
		$user=$users->name;
		$room=$rooms[$v->roomid];
		$starttime=$st[$v->starttime];
		$endtime=$et[$v->endtime];

		$objPHPExcel->getActiveSheet()->SetCellValue('A'.$i, $date);
		$objPHPExcel->getActiveSheet()->SetCellValue('B'.$i, $user);
		$objPHPExcel->getActiveSheet()->SetCellValue('C'.$i, $room);
		$objPHPExcel->getActiveSheet()->SetCellValue('D'.$i, ($starttime=="06:00 AM")?"06:00 AM":$starttime);
		$objPHPExcel->getActiveSheet()->SetCellValue('E'.$i, $endtime);

		$i++;
	}
	
	$objPHPExcel->getActiveSheet()->setTitle('Conference Reservation Details');
	
	//$objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
	//$objWriter->save(str_replace('.php', '.xlsx', __FILE__)); 
	
	header('Content-Type: application/vnd.ms-excel');
	header('Content-Disposition: attachment;filename="Conference_Reservations_Details.xls"');
	header('Cache-Control: max-age=0');
	// If you're serving to IE 9, then the following may be needed
	header('Cache-Control: max-age=1');
	// If you're serving to IE over SSL, then the following may be needed
	header ('Expires: Mon, 26 Jul 1997 05:00:00 GMT'); // Date in the past
	header ('Last-Modified: '.gmdate('D, d M Y H:i:s').' GMT'); // always modified
	header ('Cache-Control: cache, must-revalidate'); // HTTP/1.1
	header ('Pragma: public'); // HTTP/1.0
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
	$objWriter->save('php://output');

}



?>