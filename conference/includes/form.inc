<?php


function __conference_form_search($form_state){
  $form=array();
  $rooms=get_rooms();
  $format = 'Y-m-d';
  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Select date'),
    '#date_format' => $format,
    '#default_value' => variable_get('user_selected_date',date('Y-m-d')),
    '#date_label_position' => 'within',
    '#date_timezone' => 'America/Chicago',
    '#date_increment' => 15,
    '#date_year_range' => '-3:+3',
    '#datepicker_options' => array(
      'minDate' => 0,
      'maxDate'=>30
    ),
  );
  variable_set('get_rooms',$rooms);
  $form['rooms']=array(
    '#type'=>'select',
    '#title'=>'Select Conference Room',
    '#options' => variable_get('get_rooms'),
    '#description'=>'',
    '#required'=>true,
    '#default_value'=>variable_get('user_selected_room')
  );

  $form['search']=array(
    '#type'=>'submit',
    '#value' => t('search')
  );

  return $form;
}
function __conference_form_result($form_state){
  $form=array();
  $day=$sdate=variable_get('user_selected_date',date('Y-m-d'));
  $rooms=get_rooms(variable_get('user_selected_room'));
  $sroom=$rooms[variable_get('user_selected_room')];
  $day=explode('-',$day);
  $mktime=mktime(0,0,0,$day[1],$day[2],$day[0]);
  $month=date('M',$mktime);
  $date=date('dS',$mktime);
  $form['selected_info'] = array(
    '#markup' => '<div class="selected_info"> <span><b>Selected Date: </b>'.$sdate.'</span><span><b>Conference Room: </b>'.$sroom.'</span></div>'
  );
  $form['infobar'] = array(
    '#markup' => '<div class="infobar"> <span><b>Month: </b>'.$month.'</span><span><b>Date: </b>'.$date.'</span></div>'
  );
  $form['details']=array(
    '#type'=>'textarea',
    '#title'=>t('Description'),
    '#default_value' => '',
    '#description'=>'Conference Description (250 characters)',
    '#required'=>true
  );

    $form['times']=array(
      '#type'=>'checkboxes',
      '#title'=>t('Time'),
      '#options' => get_timings(),
      '#after_build' => array('__checkbox_disable'),
    );


    $form['submit']=array(
      '#type'=>'submit',
      '#value' => t('Book')
    );
    $form['reset'] = array(
      '#markup' => '<input value="Cancel" class="form-submit" type="Reset"/>',
        '#weight' => 1000
      );

  return $form;
}
function __conference_form_add_room($form_state){
  $del_arg=drupal_get_query_parameters();
  if(isset($del_arg['id']) && isset($del_arg['name'])){
    conference_delete_room($del_arg['id'],$del_arg['name']);
  }
  $form=array();
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Rooms'),
  );
  $form['basic']['roomname']=array(
    '#type'=>'textfield',
    '#title'=>'Room name',
    '#description'=>'name of the room',
    '#required'=>true
  );
  $form['basic']['search']=array(
    '#type'=>'submit',
    '#value' => t('Add')
  );

  $form['people'] = array(
    '#prefix' => '<div id="people">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Room name'), t('Action')),
    '#rows' => array(),
  );

  $v=array();
  $result = db_query("SELECT id,name FROM conference_rooms");
  foreach ($result as $row) {
    $v[$row->id]= $row->name;
  }
  variable_set('total_rooms',$v);

  $rt=array();
  $i=1;
  foreach($v as $id=>$room){
    $room_name = array(
      '#id' => 'people-' . $i . '-room_name',
      '#markup' => '<p>'.$room.'</p>',
      '#default_value' => $room,
    );
    $room_del_btn = array(
      '#id' => 'people-' . $i . '-room_name',
      '#markup' => '<a class="button" href="?id='.$id.'&name='.$room.'" value="delete" name="room_delete" id="'.$room.'">Delete</a>',
      '#default_value' => $room,
    );

    $form['people'][] = array(
      'room_name' => &$room_name,
      'room_del_btn' => &$room_del_btn,
    );

    $form['people']['#rows'][] = array(
      array('data' => &$room_name),
      array('data' => &$room_del_btn),
    );

    unset($room_name);
    unset($room_del_btn);


    $i++;
  }

  return $form;
}

function __conference_form_admin_settings($form_state){

  db_get_conference_settings();
  $form=array();
  $form['app_name']=array(
    '#type'=>'textfield',
    '#title'=>'Application Name',
    '#description'=>'',
    '#required'=>true,
    '#value'=>get_settings('application-name')
  );
  $form['advance_booking']=array(
    '#type'=>'select',
    '#title'=>'Advance hours',
    '#description'=>'',
    '#required'=>true,
    '#options' => get_advance_booking_dur(),
    '#value'=>get_settings('advance-booking')
  );
  $form['maximum_reservation']=array(
    '#type'=>'select',
    '#title'=>'Advance Days',
    '#description'=>'',
    '#required'=>true,
    '#options' => get_maximum_reservation_dur(),
    '#value'=>get_settings('max-reservation')
  );
  $form['save']=array(
    '#type'=>'submit',
    '#value' => t('save')
  );

  return $form;
}

function __conference_form_admin_rooms($form_state){
  $form=array();
  $form['room_name']=array(
    '#type'=>'textfield',
    '#title'=>'Conference Room Name',
    '#description'=>'',
    '#maxlength'=>50,
    '#required'=>true
  );
  $form['room_capacity']=array(
    '#type' => 'textfield',
    '#title'=>'Conference Room Capacity',
    '#attributes' => array(
        'data-type' => 'number'
    ),
    '#width' => '30%',
    '#align' => 'center',
    '#required' => true,
    '#maxlength' => 3
  );
  $form['room_description']=array(
    '#type'=>'textfield',
    '#title'=>'Description',
    '#description'=>'',
    '#required'=>true
  );
  $form['save']=array(
    '#type'=>'submit',
    '#value' => t('save')
  );

  return $form;
}

function __conference_form_admin_configurations_week($form_state){
  $form=array();
  $weekdays= array('Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday');
  $i=0;
  $st=get_starttimings();
  $et=get_endtimings();
  foreach($weekdays as $weekday){
    if(db_get_week_timings()){
      $dbtimings=db_get_week_timings();
    }
    $form[$weekday] = array(
        '#type' => 'fieldset',
        '#title' => t($weekday),
        '#collapsible' => TRUE,
        '#collapsed' => false,
        '#tree'=>TRUE
      );
    $form[$weekday]['weekday']=array(
      '#markup'=>'<div class="form-field-left">'.$weekday.'</div>',
      '#value'=>$weekday
    );
    $form[$weekday]['starttime']=array(
      '#type'=>'select',
      '#title'=>'Start time',
      '#description'=>'',
      '#options'=>$st,
      '#default_value'=>$dbtimings[$weekday]['st'],
      '#prefix'=>'<div class="form-field-right">'
    );
    $form[$weekday]['endtime']=array(
      '#type'=>'select',
      '#title'=>'End time',
      '#description'=>'',
      '#options'=>$et, 
      '#default_value'=>$dbtimings[$weekday]['et'],
      '#sufix'=>'</div>'
    );
    $form[$weekday]['weekday_name'] = array(
        '#type' => 'hidden',
        '#value' => $weekday,
      );
    $i++;
  }
  $form['save']=array(
    '#type'=>'submit',
    '#value' => t('save')
  );

  return $form;
}
function __conference_form_admin_configurations_month($form_state){
  $form=array();
  $i=0;
  $st=get_starttimings();
  $et=get_endtimings();
  $args=drupal_get_query_parameters();
  if(isset($args['month'])){
    $monthend=date('t',strtotime($args['month'],strtotime(date("F") . "1")));
    $monthname=date('F',strtotime($args['month'],strtotime(date("F") . "1")));
    $monthnumber=date('m',strtotime($args['month'],strtotime(date("F") . "1")));
  }
  else{
    $monthend=date('t');
    $monthname=date('F');
    $monthnumber=date('m');
  }
  

  if(isset($args['year'])){
    $monthyear=$args['year'];
  }else{
    $monthyear=date('Y');
  }


  function started_timings($date,$monthnumber){
    if(db_get_month_timings($monthnumber)){
      $dbtimings=db_get_month_timings($monthnumber);
    }
    $st_options=array();
    $selected='';
    foreach(get_starttimings() as $k=>$v){
      if(isset($dbtimings[$date]['st']) && $k==$dbtimings[$date]['st']){
        $selected='selected';
      }
      $st_options[]='<option value="'.$k.'" '.$selected.'>'.$v.'</option>';
      $selected='';
    }
    return implode('',$st_options);
  }
  
  function ended_timings($date,$monthnumber){
    if(db_get_month_timings($monthnumber)){
      $dbtimings=db_get_month_timings($monthnumber);
    }
    $et_options=array();
    $selected='';
    foreach(get_endtimings() as $k=>$v){
      if(isset($dbtimings[$date]['et']) && $k==$dbtimings[$date]['et']){
        $selected='selected';
      }
      $et_options[]='<option value="'.$k.'" '.$selected.'>'.$v.'</option>';
      $selected='';
    }
    return implode('',$et_options);
  }

  for($i=1;$i<=$monthend;$i++){
      $n=$i.'-'.$monthname.'-'.$monthyear;
      $day=date('l',strtotime($i.' '.$monthname.' '.$monthyear));
      $date=$monthyear.'-'.$monthnumber.'-'.sprintf("%02d", $i);
      $monthdate=$monthyear.'-'.$monthnumber.'-01';
      $form[$n] = array(
          '#markup' => '<fieldset class="collapsible form-wrapper collapse-processed" id="edit-'.$n.'--2"><legend><span class="fieldset-legend"><a class="fieldset-title" href="#"><span class="fieldset-legend-prefix element-invisible">Hide</span> '.$n.'</a><span class="summary"></span></span></legend><div class="fieldset-wrapper"><div class="form-field-left">'.$day.'</div><div class="form-field-right"><div class="form-item form-type-select form-item-'.$n.'-starttime"><label for="edit-'.$n.'-starttime">Start time </label><select id="edit-'.$n.'-starttime" name="'.$n.'[starttime]" class="form-select required">'.started_timings($date,$monthnumber).'</select></div><div class="form-item form-type-select form-item-'.$n.'-endtime"><label for="edit-'.$n.'-endtime">End time </label><select id="edit-'.$n.'-endtime" name="'.$n.'[endtime]" class="form-select required">'.ended_timings($date,$monthnumber).'</select></div></div></div></fieldset><input type="hidden" id="edit-save" name="'.$n.'[date]" value="'.$date.'" class="form-submit"><input type="hidden" id="edit-save" name="month-date" value="'.$monthdate.'" class="form-submit">'
        );
  }

  $form['save']=array(
    '#type'=>'submit',
    '#value' => t('save')
  );

  return $form;
}

?>