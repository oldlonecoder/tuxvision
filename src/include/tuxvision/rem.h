//
// Created by oldlonecoder on 24-12-09.
//

//#ifndef REM_H
//#define REM_H
/******************************************************************************************
 *   Copyright (C) ...,2024,2025,... by Serge Lussier                                     *
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

#include <tuxvision/colors.h>
#include <tuxvision/glyphes.h>

namespace rem
{


enum class type : tux::integers::U8{
    none, err, warning, fatal, except, message, output, debug, info, comment, syntax, status, test, interrupted, aborted, segfault, log
};

    enum class code : tux::integers::U8
    {
        ok  =0           , ///< Obviously;
        accepted         ,
        success          ,
        rejected         ,
        failed           ,
        empty            ,
        full             ,
        notempty         ,
        implemented      , ///< Like notimplemented or already implemented
        notimplemented   , ///< "Please, implement"
        untested         ,
        eof              , ///< End of file
        eos              , ///< End of stream or string or statement or Status ( machine Status )
        null_ptr         , ///< It will happen. Trust me :)
        notexist         , ///< does not exist
        exist            , ///< does already exist
        unexpected       , ///< unexpected
        expected         , ///< expected
        blocked          , ///< thread trying To lock A mutex has failed because the mutex is already locked IsIn another thread...
        locked           , ///< thread trying To lock A mutex has become the owner of the lock.
        overflow         , ///< buffer overflow
        oob              , ///< buffer overflow
        reimplement      ,
        done             ,
        complete         ,
        finish           ,
        undefined        ,
        ready            ,
        terminate        ,
        timeout          ,
        divbyzero        ,
        notvisible       ,
        cancel           ,
        object_ptr       ,
        object_id        ,

        //...

    };


    enum class fn : tux::integers::U8
    {
        func             ,
        endl             , ///< End of line Code, input format
        file             ,
        line             ,
        stamp            , ///< fully detailed timestamp
        hour             ,
        minute           ,
        seconds          ,
        weekday          ,
        dayname          ,
        month            , ///< Par defaut month name
        day              ,
        monthnum         , ///< Maintenant explicite
        year
    };

    enum class action : tux::integers::U8
    {
        enter            , ///< enter bloc or indent
        leave            , ///< End (logger: End of entry accumulators, so do commit); End of (sel)Section, Attribute ( auto-color::reset ) and unindent
        commit           , ///< Commit...
        begin            , ///< Begin (sel)Section or augment indent level
        end              , ///< End   Loop/Action or End selection or Unindent one level; or end inputs on the current logentry then flush to file.
        continu          , ///< Continue Loop or continue Signals::Action iteration.
        dismiss          , ///< The current action or iteration has been dismissed : break loop or actual action.
    };

std::string to_string(rem::type ty);
std::string to_string(rem::code cod);
std::string to_string(rem::fn fn);
std::string to_string(rem::action a);
std::pair<tux::glyph::type, tux::ui::color::pair> type_attributes(rem::type ty);
std::pair<tux::glyph::type, tux::ui::color::pair> return_code_attributes(rem::code cod);
std::pair<tux::glyph::type, tux::ui::color::pair> function_attributes(rem::fn fn);
std::pair<tux::glyph::type, tux::ui::color::pair> action_attributes(rem::action a);

}



bool operator !(rem::code);
