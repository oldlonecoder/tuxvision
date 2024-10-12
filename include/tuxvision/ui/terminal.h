//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include <tuxvision/ui/globals.h>
#include <tuxvision/journal/book.h>
#include <tuxvision/tools/signals.h>

namespace tux::ui::terminal
{


rectangle geometry();





//static terminal& instance();
//terminal();

book::code begin();
book::code end();


void switch_alternate();
void switch_back();

void start_mouse();
void stop_mouse();

void clear();
void cursor_off();
void cursor_on();
void cursor(ui::cxy xy);
void mv(globals::direction::type dir=globals::direction::right, int d=1);

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
signals::action<rectangle> _winch;



}
