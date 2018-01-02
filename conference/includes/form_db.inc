<?php

function get_rooms(){
  $v=array();
  $result = db_query("SELECT id,name FROM conference_rooms");
  foreach ($result as $row) {
    $v[$row->id]= $row->name;
  }
  return $v;
}

function db_get_room_desc($room=''){
  if($room!=='')
    $q="SELECT * FROM conference_rooms WHERE name='".$room."'";
  else
    $q="SELECT * FROM conference_rooms";
  $result = db_query($q);
  return $result->fetch()->description;
}
function db_get_room_name(){
    $q="SELECT * FROM conference_rooms";
  $result = db_query($q);
  return $result->fetch()->name;
}

function get_timings(){
  $t=array();
  $result = db_query("SELECT id,timing FROM conference_timings");
  foreach ($result as $row) {
    $t[$row->id]= $row->timing; 
  }
  return $t;
}

function get_advance_booking_dur(){
  return array(
    30=>30,
    60=>60,
    90=>90,
    120=>120,
    150=>150,
    180=>180,
    210=>210,
    240=>240,
    270=>270,
    300=>300,
    330=>330,
    360=>360,
    390=>390,
    420=>420,
    450=>450,
    480=>480,
    510=>510,
    540=>540,
    570=>570,
    600=>600,
  );
}

function get_maximum_reservation_dur(){
  return array(
    30=>30,
    60=>60,
    90=>90,
    120=>120,
    150=>150,
    180=>180,
    210=>210,
    240=>240,
    270=>270,
    300=>300,
    330=>330,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
    360=>360,
  );
}

function get_starttimings(){

  return array(
    '083000'=>'08:30 AM',
    '090000'=>'09:00 AM',
    '093000'=>'09:30 AM',
    '100000'=>'10:00 AM',
    '103000'=>'10:30 AM',
    '110000'=>'11:00 AM',
    '113000'=>'11:30 AM',
    '120000'=>'12:00 PM',
    '123000'=>'12:30 PM',
    '130000'=>'01:00 PM',
    '133000'=>'01:30 PM',
    '140000'=>'02:00 PM',
    '143000'=>'02:30 PM',
    '150000'=>'03:00 PM',
    '153000'=>'03:30 PM',
    '160000'=>'04:00 PM',
    '163000'=>'04:30 PM',
    '170000'=>'05:00 PM',
    '173000'=>'05:30 PM',
    '180000'=>'06:00 PM',
    '999999'=>'Holiday'
  );
}
function get_endtimings(){

  return array(
    '083000'=>'08:30 AM',
    '090000'=>'09:00 AM',
    '093000'=>'09:30 AM',
    '100000'=>'10:00 AM',
    '103000'=>'10:30 AM',
    '110000'=>'11:00 AM',
    '113000'=>'11:30 AM',
    '120000'=>'12:00 PM',
    '123000'=>'12:30 PM',
    '130000'=>'01:00 PM',
    '133000'=>'01:30 PM',
    '140000'=>'02:00 PM',
    '143000'=>'02:30 PM',
    '150000'=>'03:00 PM',
    '153000'=>'03:30 PM',
    '160000'=>'04:00 PM',
    '163000'=>'04:30 PM',
    '170000'=>'05:00 PM',
    '173000'=>'05:30 PM',
    '180000'=>'06:00 PM',
    '999999'=>'Holiday'
  );
}

function db_get_month_timings($monthnum){
  $q = db_query("SELECT * FROM conference_month_hours WHERE DATE_FORMAT(month,'%m')=".$monthnum);
  $qq= $q->fetchAll();
  $t=array();
  foreach( $qq as $item=>$val){
    $t[$val->month]=array(
      'st'=>$val->starttime,
      'et'=>$val->endtime,
    );
  }
  return $t;
}

function db_get_week_timings(){
  $q = db_query("SELECT * FROM conference_week_hours");
  $qq= $q->fetchAll();
  $t=array();
  foreach( $qq as $item=>$val){
    $t[$val->weekday]=array(
      'st'=>$val->starttime,
      'et'=>$val->endtime,
    );
  }
  return $t;
}

function db_get_conference_settings(){
  $q = db_query("SELECT * FROM conference_settings");
  $settings=array();
  foreach( $q->fetchAll() as $item=>$val){
    $settings[$val->name]=$val->value;
  }
  variable_set('settings', $settings);
}


function db_get_settings(){
  $q = db_query("SELECT * FROM conference_settings");
  foreach( $q->fetchAll() as $item=>$val){
    $settings[$val->name]=$val->value;
  }
  return $settings;
}

function db_get_max_booking_hours(){
  $q = db_query("SELECT * FROM conference_settings WHERE name='max-reservation'");
    $qq=$q->fetch();
    return $qq->value;
}
function db_get_advance_booking_hours(){
  $q = db_query("SELECT * FROM conference_settings WHERE name='advance-booking'");
    $qq=$q->fetch();
    return $qq->value;
}

function get_settings($name){
  $settings=variable_get('settings',array());
  return (isset($settings[$name]))?$settings[$name]:'';
}

function db_add_booking($userid,$roomid,$starttime,$endtime,$date,$details){
  $fs=db_insert('conference_booking')
    ->fields(array(
      'userid'=>$userid,
      'roomid'=>$roomid,
      'starttime'=>$starttime,
      'endtime'=>$endtime,
      'date'=>$date, 
      'details'=>$details,  
    ))->execute();
  return $fs;
}
function is_room_booked($roomid,$timeids,$date){
 $q = db_query("SELECT * FROM conference_booking WHERE starttime IN (".implode(',',$timeids).") AND date='".$date."' AND roomid=".$roomid);
   return $q->fetch();
}

function is_another_room_booked($roomid,$userid,$timeids,$date){  
  $result = db_query("SELECT * FROM conference_booking WHERE starttime IN (".implode(',',$timeids).") AND date='".$date."' AND userid=".$userid." AND roomid !=".$roomid);
  if($result->rowCount()>=1)
    return true;
  return false;
}
function is_middletime_room_booked($st,$date,$roomid){
  $r=db_query("SELECT * FROM `conference_booking` WHERE '".$st."'>starttime && '".$st."'<endtime && date='".$date."' && roomid='".$roomid."'");
  if($r->rowCount()>=1)
    return true;
  return false;
}

function is_another_room_booked_middletime($st,$date,$roomid,$userid){
  $r=db_query("SELECT * FROM `conference_booking` WHERE '".$st."'>starttime && '".$st."'<endtime && date='".$date."' && userid=".$userid);
  if($r->rowCount()>=1)
    return true;
  return false;
}

function is_admin($user){
  return in_array('administrator', array_values($user->roles)) ;
}

function db_delete_booked_room($id){
  $dq = db_delete('conference_booking')
    ->condition('id', $id)
    ->execute();
  return $dq;
}
function db_delete_user_booked_room($uid,$id){
  $r=db_query("SELECT * FROM `conference_booking` WHERE id='".$id."' && userid=".$uid);
  if($r->rowCount()==0)
    return false;
  $dq = db_delete('conference_booking')
    ->condition('userid', $uid)
    ->condition('id', $id)
    ->execute();
  return $dq;
}

function db_add_room($values){
  $fs=db_insert('conference_rooms')
    ->fields(array(
      'name'=>$values['room_name'],
      'capacity'=>$values['room_capacity'],
      'description'=>$values['room_description'],
    ))->execute();
  return $fs;
}

function get_reserved_details(){
  $result = db_query("SELECT * FROM conference_booking ORDER By date DESC, roomid, starttime");
  return $result->fetchAll();
}
function get_reserved_details_bydate($date){
  $result = db_query("SELECT * FROM conference_booking WHERE date='".$date."' ORDER By date DESC, roomid, starttime");
  return $result->fetchAll();
}
function get_reserved_details_btwdate($fdate,$tdate){
  $result = db_query("SELECT * FROM conference_booking WHERE date>='".$fdate."' AND date<='".$tdate."' ORDER By date");
  return $result->fetchAll();
}
function get_reserved_details_byroom($rid){
  $result = db_query("SELECT * FROM conference_booking WHERE roomid='".$rid."' ORDER By date");
  return $result->fetchAll();
}
function get_user_reserved_details($uid){
  $result = db_query("SELECT * FROM conference_booking WHERE userid='".$uid."' ORDER By date DESC" );
  return $result->fetchAll();
}
function get_rooms_details(){
  $result = db_query("SELECT * FROM conference_rooms");
  return $result->fetchAll();
}
function get_reserved_details_ajax(){
  $rooms=get_rooms();
  $st_name=get_starttimings();
  $et_name=get_endtimings();
  $result = db_query("SELECT *,userid AS `title`,date AS `start` FROM conference_booking");
  $i=0;
  $qq=array();
  foreach($result as $q=>$r){
    $uid=user_load($r->title);
    $qq[$i]['title']=strtoupper($uid->name).' '.$rooms[$r->roomid];
    $qq[$i]['start']=$r->start.'T'.$r->starttime[0].$r->starttime[1].':'.$r->starttime[2].$r->starttime[3].':'.$r->starttime[4].$r->starttime[5];
    $qq[$i]['end']=$r->start.'T'.$r->endtime[0].$r->endtime[1].':'.$r->endtime[2].$r->endtime[3].':'.$r->endtime[4].$r->endtime[5];
    $qq[$i]['username']=strtoupper($uid->name);
    $qq[$i]['date']=$r->start;
    $qq[$i]['st']=$r->starttime;
    $qq[$i]['st_name']=$st_name[$r->starttime];
    $qq[$i]['et']=$r->endtime;
    $qq[$i]['et_name']=$et_name[$r->endtime];
    $qq[$i]['desc']=$r->details;
    $qq[$i]['eid']=$r->id;
    $i++;
  }
  return json_encode($qq);
}