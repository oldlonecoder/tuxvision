//
// Created by oldlonecoder on 24-11-12.
//

#include <tuxvision/tui/io/ansi_colours_parser.h>

/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the Code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

//#pragma once




namespace tux::io
{
ansi_colours_parser::ansi_colours_parser()
{
}


ansi_colours_parser::~ansi_colours_parser()
{
}


rem::code ansi_colours_parser::parse(const std::string& _filename)
{
    _filename_ = _filename;
    _file.open(_filename_, std::ios::in);
    if (!_file.is_open())
    {
        throw log::exception()
        [
            log::except() << class_name() << "::ansi_colours_parser::parse(): error opening file " << _filename_ << log::eol
        ];
    }

    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::parse_text(const std::string& _text)
{
    _text_ = _text;
    _input_ = _text_.cbegin();
    _line.clear();
    _cursor = _line.begin();
    //...

    return rem::code::notimplemented;
}


/*!
 * @brief Opes file that has encoded ansi colours sequences with its contents.
 * @param _filename
 * @return
 */
rem::code ansi_colours_parser::parse(const char* _filename)
{
    _filename_ = _filename;
    _file.open(_filename, std::ios::in);
    if (!_file.is_open())
        throw log::exception() [
            log::except() << color::lightsteelblue3 <<  class_name() << color::reset << ": error opening file " << color::hotpink4 << _filename_ << log::eol
            ];
    _lines_.clear();
    for(std::string line; std::getline(_file, line);) _lines_.emplace_back(line);
    _file.close();
    return rem::code::success;
}




// -----------------------------------------------------------------------------
// Parsers:
//------------------------------------------------------------------------------
#pragma region parsers

auto ansi_colours_parser::_fg() -> rem::code
{
    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::_bg()
{
    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::_rgb()
{
    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::_eos()
{
    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::_scan_num()
{
    return rem::code::notimplemented;
}


rem::code ansi_colours_parser::_string()
{
    return rem::code::notimplemented;
}


#pragma endregion parsers
}//
