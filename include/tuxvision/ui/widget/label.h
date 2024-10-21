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


#include <tuxvision/ui/widget_base.h>


namespace tux::ui
{

class label : public widget_base 
{
    std::string _text_{};
public:
    label();
    ~label() override;
    label(object* _parent_widget, const std::string _id);
    book::code set_text(const std::string& _txt);

    book::code draw() override;    

};
}