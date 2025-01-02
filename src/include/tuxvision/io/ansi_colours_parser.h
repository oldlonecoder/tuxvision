//
// Created by oldlonecoder on 24-11-12.
//

//#ifndef ANSI_PARSER_H
//#define ANSI_PARSER_H
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

#include <fstream>
#include <tuxsl++/sscan.h>
#include <tuxsl++/tui/vchar.h>

namespace tux::io
{

class TUXSL_API ansi_colours_parser
{
    CLASSNAME_START(ansi_colours_parser)
    tux::ui::terminal::vchar::back_buffer _product_{nullptr};
    std::string _filename_{};
    std::string _text_{};
    std::vector<std::string> _lines_{};
    std::ifstream _file{};
    std::mutex _log_lock{};
    tux::ui::terminal::vchar::string _line{};
    tux::ui::terminal::vchar::string::iterator _cursor{};
    std::string::const_iterator _input_{};
    tux::ui::terminal::vchar::back_buffer _production_{nullptr};
    //tux::ui::terminal::vchar::
public:
    ansi_colours_parser();
    virtual ~ansi_colours_parser();

    rem::code parse(const char* _filename);
    rem::code parse(const std::string& _filename);
    rem::code parse_text(const std::string& _text);


private:

    #pragma region parsers
    using parser = auto(ansi_colours_parser::*)()->rem::code; // hehehe yeah it compiles ;)

    auto _fg() -> rem::code;
    auto _bg() -> rem::code;;
    auto _rgb() -> rem::code;;
    auto _eos() -> rem::code;; // End Of Sequence
    auto _string() -> rem::code;
    auto _scan_num() -> rem::code;
    #pragma endregion parsers
};


} // namespace tux::io
