<?php


function is_user_prebooked_other_room($userid,$date,$timeid,$roomid){
  $result = db_query("SELECT * FROM conference_booking WHERE timeid=".$timeid." AND date='".$date."' AND userid=".$userid." AND roomid !=".$roomid);
  return $result->fetch();
}
function is_room_booked_($timeid,$date,$userid,$roomid){
  $q = db_query("SELECT * FROM conference_booking WHERE timeid=".$timeid." AND date='".$date."' AND userid=".$userid." AND roomid=".$roomid);
  return $q->fetch();
}
function is_room_booked_byuser($timeid,$date,$roomid){
  $q = db_query("SELECT * FROM conference_booking WHERE timeid=".$timeid." AND date='".$date."' AND roomid=".$roomid." LIMIT 1");
  return $q->fetch();
}
