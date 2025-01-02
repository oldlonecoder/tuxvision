//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge Lussier                                          *
 *   serge.lussier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/


#pragma once

#include <tuxvision/_start_.h>
#include <tuxvision/tui/tuxns.h>
#include <tuxvision/journal/logger.h>
#include <tuxvision/actions.h>
#include <tuxvision/object.h>

namespace tux::ui::terminal
{


rectangle geometry();



rem::code query_winch();

//static terminal& instance();
//terminal();

rem::code begin();
rem::code end();


void switch_alternate();
void switch_back();

void start_mouse();
void stop_mouse();

void clear();
void cursor_off();
void cursor_on();
void cursor(ui::cxy xy);
void mv(ui::direction::type dir=ui::direction::right, int d=1);

enum caret_shapes: u8
{
    def,
    bloc_blink,
    block_steady,
    under_blink,
    under_steady,
    vbar_blink,
    vbar_steady
};

enum class DECMode
{
    line_wrap               = 7,
    crs_shape               = 25,
    mouse_X10               = 9,
    mouse_vt200             = 1000,
    mouse_vt200_hl          = 1001,
    mouse_report_buttons    = 1002,
    mouse_report_any        = 1003,
    mouse_utf8              = 1005,
    mouse_sgr_ext_mode      = 1006,
    mouse_urxvt_mode        = 1015,
    mouse_sgr_pixel_mode    = 1016,
    alternate_screen        = 1049
};

constexpr const char* CSI = "\x1b[";
signals::action<rectangle>& term_resize_signal();
rem::code update_widget(object* _widget);


}
