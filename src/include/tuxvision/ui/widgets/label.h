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


#include <tuxvision/tui/widgets/icon.h>


namespace tux::ui
{

class label : public widget
{
    CLASSNAME(label)
    std::string _text_{};

    ui::justify::Type _justify_{ui::justify::auto_size};
    ui::cxy _text_xy_{0,0};
    icon* _prefix_icon_{nullptr};
    icon* _suffix_icon_{nullptr};
public:
    label()=default;
    ~label() override;
    label(object* _parent_widget, const std::string& _id);
    rem::code set_text(const std::string& _txt);
    rem::code set_justify(ui::justify::Type j);
    rem::code draw() override;

    rem::code setup_ui() override;
    rem::code set_prefix_icon(glyph::type ic_id);
    rem::code set_suffix_icon(glyph::type ic_id);


};
}
