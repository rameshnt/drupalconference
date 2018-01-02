(function($) {

    $(document).ready(function() {


        var caldr = $('#calendar').fullCalendar({
            header: {
                left: 'prev,next today',
                center: 'title',
                right: 'month,agendaWeek,agendaDay'
            },
            defaultView: 'agendaWeek',
            dayRender: function(date, cell) {
                $(cell).addClass('cell-freeze');
                var tdays = $('.advance-booking-variable').attr('id');
                if (date.format('YYYYMMDD') >= moment().format('YYYYMMDD') && date.format('YYYYMMDD') <= moment().add(tdays, 'days').format('YYYYMMDD')) {
                    $(cell).removeClass('cell-freeze').addClass('day-select-enabled');
                }
            },
            editable: false,
            eventLimit: true,
            dayClick: function(date, jsEvent, view) {
                console.log(date);
                var day_selected = false;
                if (view.name == "month") {
                    $('#calendar').fullCalendar('changeView', "agendaDay");
                    var view = $('#calendar').fullCalendar('getView');
                    view.start = date;
                }

                if (view.name == "agendaWeek" || view.name == "agendaDay") {
                    $('#calendar').fullCalendar('gotoDate', date)
                    var tdays = $('.advance-booking-variable').attr('id');
                    if ((date.format('YYYYMMDD') >= moment().format('YYYYMMDD')) && (date.format('YYYYMMDD') <= moment().add(tdays, 'days').format('YYYYMMDD'))) {
                        day_selected = true;
                    } else {
                        day_selected = false;

                    }
                }
                $('#booking-form .msg').removeClass('wait').removeClass('success').removeClass('error').text('');
                $('#booking-form .date-field').val(format_date(date.format()));
                if ((view.name == 'month' && $(this).hasClass('cell-freeze')) || (view.name == 'agendaWeek' && day_selected == false) || (view.name == 'agendaDay' && day_selected == false)) {
                    $('#error-alert .msg-container').text('');
                    $('#error-alert .msg-container').text('Invalid Date selected.');
                    $('#error-alert').modal({
                        fadeDuration: 100
                    });
                } else if ($('.cf-admin-calender-content').hasClass('user-not-logged')) {
                    $('#error-alert .msg-container').text('');
                    $('#error-alert .msg-container').html('Please <a href="/user/login">login</a> to book the room');
                    $('#error-alert').modal({
                        fadeDuration: 100
                    });
                } else {
                    $('#booking-form').modal({
                        fadeDuration: 100
                    });
                }
            },
            eventClick: function(calEvent, jsEvent, view) {
              console.log(calEvent.date);
                $('#cancel-form .msg').removeClass('wait').removeClass('success').removeClass('error').text('');
                $('#cancel-form .date-field').val(calEvent.date);
                $('#cancel-form .booked_by').text(calEvent.username);
                $('#cancel-form .start-time option').remove();
                $('#cancel-form .end-time option').remove();
                $('#cancel-form .start-time ').append($('<option></option>').val(calEvent.st).html(calEvent.st_name));
                $('#cancel-form .end-time ').append($('<option></option>').val(calEvent.et).html(calEvent.et_name));
                $('#cancel-form textarea').val(calEvent.desc);
                $('#cancel-form .eid-variable').attr('id', calEvent.eid);
                $('#cancel-form .id-variable').attr('id', calEvent._id);
                $('#cancel-form').modal({
                    fadeDuration: 100
                });
                return false;
            }
        });

        $.ajax({
            url: Drupal.settings.basePath + 'conference/ajax/getreserved/',
            success: function(data) {
                var d = $(data).find('#block-system-main .content:first').text();
                var j = JSON.parse(d);
                for (i = 0; i < j.length; i++) {
                    $('#calendar').fullCalendar('renderEvent', j[i], true);
                }
            }
        });
        $('.form-ajax-submit .save').click(function() {
            book_room(
                $('#booking-form .date-field').val(),
                $('#booking-form .start-time option:selected').val(),
                $('#booking-form .end-time option:selected').val(),
                $('#booking-form textarea').val(),
                $('#booking-form .start-time option:selected').text()
            );
        });
        $('#cancel-form .remove-booking').click(function() {
            remove_booking($('#cancel-form .eid-variable').attr('id'));
            $('#calendar').fullCalendar('removeEvents', $('#cancel-form .id-variable').attr('id'));
        });
        $('#cancel-form .remove-user-booking').click(function() {
            remove_user_booking($('#cancel-form .eid-variable').attr('id'));
        });

        $('button.cancel').click(function() {
            $.modal.close();
        });
        $('#booking-form .date-field').change(function() {
            // $('#booking-form .start-time,#booking-form .end-time').attr('disabled','disabled');
        });


        // calendar 


        var calendar_selected_room = $('.calender-wrap .calender-room select').val();
        $('.calender-wrap .calender-room select').change(function() {
            calendar_selected_room = $(this).val();
        });


    });

    function format_date(date_str) {
        date_str = date_str.split('T');
        return date_str[0];
    }

    function get_booking_response(date, st, et, desc) {
        var link = Drupal.settings.basePath + 'conference/ajax/';
        $.ajax({
            type: 'GET',
            url: link,
            data: 'date=' + date + '&st=' + st + '&et=' + et + '&desc=' + desc + '&room=' + calendar_selected_room,
            success: function(data) {
                var res = $(data).find('#block-system-main .content:first').text();
                res = JSON.parse(res);
                $('#booking-form .msg').text(res['msg']).removeClass('error').addClass(res['type']);
            },
            error: function(data) {
                var res = 'Error. please try again after some time.';
                $('#booking-form .msg').text(res);
            }
        });
        $('#booking-form .msg').addClass('wait').text('wait..');
    }


    function book_room(date, st, et, desc,st_text) {
        var link = Drupal.settings.basePath + 'conference/ajax/add_ajax_event';
        var calendar_selected_room = $('.calender-wrap .calender-room select').val();
        $.ajax({
            type: 'GET',
            url: link,
            data: 'date=' + date + '&st=' + st + '&et=' + et + '&desc=' + desc + '&roomid=' + calendar_selected_room,
            success: function(data) {
                var res = $(data).find('#block-system-main .content:first').text();
                res = JSON.parse(res);
                if (res['login'])
                    res['msg'] = 'Please <a href="/user/login">login</a> to book your room!';
                $('#booking-form .msg').html(res['msg']).removeClass('error').addClass(res['type']);
                if (res['type'] == 'success') {
                    var event = {
                        title: res['username'] + ' ' + calendar_selected_room,
                        date: date,
                        desc:desc,
                        st_name:st_text,
                        et_name:et,
                    };

                    $('#calendar').fullCalendar('renderEvent', event, true);
                }
            },
            error: function(data) {
                var res = 'Error. please try again after some time.';
                $('#booking-form .msg').text(res);
            }
        });
        $('#booking-form .msg').addClass('wait').text('wait..');
    }

    function remove_booking(id) {
        $('#cancel-form .msg').removeClass('success').removeClass('wait').removeClass('error').text('');
        var link = Drupal.settings.basePath + 'conference/ajax/remove_ajax_event';
        $.ajax({
            type: 'GET',
            url: link,
            data: 'eid=' + id,
            success: function(data) {
                var res = $(data).find('#block-system-main .content:first').text();
                res = JSON.parse(res);
                $('#cancel-form .msg').text(res['msg']).addClass(res['type']);
            },
            error: function(data) {
                var res = 'Error. please try again after some time.';
                $('#cancel-form .msg').text(res);
            }
        });
        $('#cancel-form .msg').addClass('wait').text('wait..');
    }
    function remove_user_booking(id) {
        $('#cancel-form .msg').removeClass('success').removeClass('wait').removeClass('error').text('');
        var link = Drupal.settings.basePath + 'conference/ajax/remove_user_ajax_event';
        $.ajax({
            type: 'GET',
            url: link,
            data: 'eid=' + id,
            success: function(data) {
                var res = $(data).find('#block-system-main .content:first').text();
                res = JSON.parse(res);
                $('#cancel-form .msg').text(res['msg']).addClass(res['type']);
                if(res['type']!="error"){
                  $('#calendar').fullCalendar('removeEvents', $('#cancel-form .id-variable').attr('id'));
                }
            },
            error: function(data) {
                var res = 'Error. please try again after some time.';
                $('#cancel-form .msg').text(res);
            }
        });
        $('#cancel-form .msg').addClass('wait').text('wait..');
    }


}(jQuery));