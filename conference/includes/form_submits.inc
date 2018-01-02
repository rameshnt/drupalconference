<?php


function __conference_form_search_submit($form, $form_state){
  global $user;
  variable_set('user_selected_room',$form_state['values']['rooms']);
  variable_set('user_selected_date',$form_state['values']['date']);
  $form_state['redirect'] = 'conference';
}

function __conference_form_result_submit($form, $form_state){
  global $user;
  $timings=$form_state['values']['times'];
  $details=$form_state['values']['details'];
  $roomid=variable_get('user_selected_room');
  $date=variable_get('user_selected_date');
  $rooms=get_rooms();
  $timings_array=get_timings();
foreach($timings as $key=>$timeid) {

  if (is_admin($user) && $timeid == 0) {
    $already_booked = is_room_booked($roomid, $key, $date);
    if ($already_booked) {
      db_delete_booked_room($roomid, $key, $date);
      drupal_set_message(t('Booking canceled for '.$rooms[$roomid]));
    }
  }
}
  $booking_added=false;
  foreach($timings as $timeid){

    if($timeid !=0){

      $already_booked=is_room_booked($roomid,$timeid,$date);
      if($already_booked ){
        drupal_set_message(t('Selected room already booked at the same time of '.$timeid));
        break;
      }
      $another_room_booked=is_another_room_booked($roomid,$user->uid,$timeid,$date);
      if($another_room_booked){
        drupal_set_message(t('You have already booked another room at the same time of '.$timings_array[$timeid]),'warning');
        break;
      }
      $q=db_add_booking($user->uid,$roomid,$timeid,$date,$details);
      $booking_added=$q;
    }
  }
  if($booking_added)
  drupal_set_message('Room '.$rooms[variable_get('user_selected_room')].' booked');
}

function __conference_form_add_room_submit($form, &$form_state){
  if(in_array($form_state['values']['roomname'],variable_get('total_rooms'))){
    form_set_error('name', t('Room name already exists'));
    return $form;
  }
  db_add_room($form_state['values']['roomname']);
  drupal_set_message('new room added. '.$form_state['values']['roomname']);
}


function __conference_form_admin_configurations_week_submit($form, &$form_state){
  $dq = db_delete('conference_week_hours')
    ->execute();

    foreach($form_state['values'] as $k=>$v){
      if(!is_array($v))
        break;
      $aq=db_insert('conference_week_hours')
      ->fields(array(
        'weekday'=>$k,
        'starttime'=>$v['starttime'],
        'endtime'=>$v['endtime'],
      ))->execute();
    }
      if($aq)
       drupal_set_message('Updated.');
}

function __conference_form_admin_configurations_month_submit($form, &$form_state){
  $dq = db_query("DELETE FROM conference_month_hours WHERE DATE_FORMAT(month,'%y%m')=DATE_FORMAT('".$form_state['input']['month-date']."','%y%m')");

    foreach($form_state['input'] as $k=>$v){
      if(is_array($v) && $k!='dp'){
        $aq=db_insert('conference_month_hours')
        ->fields(array(
          'month'=>$form_state['input'][$k]['date'],
          'starttime'=>$form_state['input'][$k]['starttime'],
          'endtime'=>$form_state['input'][$k]['endtime'],
        ))->execute();
      }
     }
       drupal_set_message('Updated.');
}


function conference_delete_room($rid, $rname){
  $num_deleted = db_delete('conference_rooms')
    ->condition('id', $rid)
    ->execute();
    $rc=db_delete('conference_booking')
    ->condition('roomid', $rid)
    ->execute();
    if($num_deleted)
     drupal_set_message('Room '. $rname.' Deleted');
}

function delete_room_by_admin($roomid,$timeid,$dateid){

  $num_deleted = db_delete('conference_booking')
    ->condition('roomid', $roomid)
    ->condition('timeid', $timeid)
    ->condition('date', $dateid)
    ->execute();
    if($num_deleted)
     drupal_set_message('Room booking removed');
}

function __conference_form_admin_settings_submit($form, $form_state){
  $update = db_update('conference_settings') // Table name no longer needs {}
    ->fields(array(
      'value' => $form_state['input']['app_name'],
    ))
    ->condition('name', 'application-name', '=')
    ->execute();
  $update = db_update('conference_settings') // Table name no longer needs {}
    ->fields(array(
      'value' => $form_state['input']['advance_booking'],
    ))
    ->condition('name', 'advance-booking', '=')
    ->execute();
  $update = db_update('conference_settings') // Table name no longer needs {}
    ->fields(array(
      'value' => $form_state['input']['maximum_reservation'],
    ))
    ->condition('name', 'max-reservation', '=')
    ->execute();
    drupal_set_message('Settings Saved.');
}
function __conference_form_admin_rooms_submit($form, $form_state){
  $form_state['values']['room_capacity']=(int)$form_state['values']['room_capacity'];
  if(in_array($form_state['values']['room_name'],get_rooms())){
      form_set_error('name', t('Room name already exists'));
      return $form;
    }
    db_add_room($form_state['values']);
    drupal_set_message('new room added. '.$form_state['values']['room_name']);

}


?>
