<?php


function __set_date_range($form_element, $form_values){
  $from=array(
    'y'=>date('Y'),
    'm'=>date('m'),
    'd'=>date('d')
    );
  $to=array(
    'y'=>date('Y', strtotime("+30 days")),
    'm'=>date('m', strtotime("+30 days")),
    'd'=>date('d', strtotime("+30 days"))
    );

  if($from['y']!=$to['y'])
    $year=$from['y'];
  else
    $year=range($from['y'], $to['y']);

  $month=range($from['m'], $to['m']);
  // $month=array();
  // foreach($month_r as $d=>$v){
  //   $month[$d]=date('M',mktime(0,0,0,$v));
  // }
  $day=array();
  for($i=0;$i<=30;$i++){
    $day[$i]=date('d', strtotime("+".$i." days"));
  }

  $form_element['year']['#options'] = drupal_map_assoc($year);
  $form_element['month']['#options'] = drupal_map_assoc($month);
  $form_element['day']['#options'] = drupal_map_assoc($day);

  asort($form_element);
  return $form_element;
}
function __checkbox_disable($form_element, $form_values){
    $date=variable_get('user_selected_date');
    $room=variable_get('user_selected_room');
    foreach($form_element['#options'] as $id=>$name){
      $result = db_query("SELECT * FROM conference_booking WHERE timeid=".$id." AND date='".$date."' AND roomid =".$room);
      global $user;
      $admin=false;
      $admin_class='';
      $checked_inputs=array();
        if (in_array('administrator', array_values($user->roles))) {
          $admin=true;
          $admin_class='is_admin';
        }
      if($result->fetch()){
        $uid = db_query("SELECT userid FROM conference_booking WHERE timeid=".$id." AND date='".$date."' LIMIT 1");
        $user=user_load($uid->fetch()->userid);
        $username=$user->name;

          $form_element[$id]['#checked'] = TRUE;
        $form_element[$id]['#attributes']['id'] = $admin_class;
          if(!$admin)
            $form_element[$id]['#attributes']['disabled'] = 'disabled';
        $form_element[$id]['#title']='<span class="l '.$admin_class.'">'.$form_element[$id]['#title'].'</span><span class="n '.$admin_class.'">'.$username.'</span>';
        $checked_inputs[$id]=(string)$id;
        $_SESSION['checked_inputs'][$id]=$id;
      }
    }
    return $form_element;
}

?>
