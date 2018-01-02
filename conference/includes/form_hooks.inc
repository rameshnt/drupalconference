<?php

function __conference_admin_rooms_page_form() {
  return drupal_get_form('__conference_form_add_room');
}
function __conference_booking_page_form() {
  return array(drupal_get_form('__conference_form_search'),drupal_get_form('__conference_form_result'));
}

?>
