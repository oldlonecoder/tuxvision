//
// Created by oldlonecoder on 8/21/24.
//

//#ifndef log_H
//#define log_H
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

#include <tuxvision/glyphes.h>
#include <tuxvision/string.h>

#include <source_location>

#include <functional>
#include <tuxvision/geometry.h>
#include <filesystem>
#include <fstream>



using tux::ui::color;
using tux::glyph;
using tux::accent_fr;
using tux::ui::rectangle;
using tux::ui::cxy;
using tux::ui::size;


namespace tux
{

class object;

class OOM_API log
{
public:
    struct header_component
    {
         integers::U8 type  : 1;
         integers::U8 stamp : 1;
         integers::U8 fun  : 1;
         integers::U8 file  : 1;
         integers::U8 line  : 1;
         integers::U8 col   : 1;
       //...
    };


private:
    rem::type _type_{};
    rem::code _code_{};
    std::source_location location{};
    // ---------------------------------
    tux::string  text{};

    header_component _headercomp_{1,0,1,1,1,1};
public:


    enum oef : integers::u8 { eol,endl };
    static log& error       (std::source_location src = std::source_location::current());
    static log& warning     (std::source_location src = std::source_location::current());
    static log& fatal       (std::source_location src = std::source_location::current());
    static log& except      (std::source_location src = std::source_location::current());
    static log& message     (std::source_location src = std::source_location::current());
    static log& write       (std::source_location src = std::source_location::current());
    static log& debug       (std::source_location src = std::source_location::current());
    static log& info        (std::source_location src = std::source_location::current());
    static log& comment     (std::source_location src = std::source_location::current());
    static log& syntax      (std::source_location src = std::source_location::current());
    static log& status      (std::source_location src = std::source_location::current());
    static log& test        (std::source_location src = std::source_location::current());
    static log& interrupted (std::source_location src = std::source_location::current());
    static log& aborted     (std::source_location src = std::source_location::current());
    static log& segfault    (std::source_location src = std::source_location::current());
    static log& jnl         (std::source_location src = std::source_location::current());

    log& operator << (const std::string& txt);
    log& operator << (const char* txt);
    log& operator << (char ch);
    log& operator << (std::string_view txt);
    log& operator << (const rectangle& r);
    log& operator << (ui::cxy pt);
    log& operator << (color::code col);
    log& operator << (color::pair fgbg);
    log& operator << (glyph::type gh);
    log& operator << (accent_fr::type ac);
    log& operator << (rem::type ty);
    log& operator << (rem::code cod);
    log& operator << (rem::fn fn);
    log& operator << (rem::action a_action);
    log& operator << (const tux::string::list& _list );
    log& operator << (log::oef e);
    template<typename T> log& operator << (const T& v)
    {
        text << v;
        return *this;
    }

    static std::vector<log> _mem_stream_;
    void init_header();
    static void purge(const std::function<void(log &)>& f);

    struct OOM_API section
    {
        std::string id;   ///< Section ID which is also the base name of the output file.
        std::ofstream ofs;
        std::ofstream* fout{nullptr}; ///< will see.
        std::vector<log> contents{}; ///< Actual contents. Will be transferred into the entries container when the transition is done.
       // std::vector<log::logentry> entries{}; ///< in transition.
        using list = std::vector<log::section>;
        log::section& operator *(); ///< Get the reference of this instance.
        log& operator << (log&& r); // move operator, will be removed and replaced with log::logentry object.
        void remove_last();
        void close();

    };


    static log::section::list sections;
    static log::section::list::iterator current_section;
public:
    static log::section& select(const std::string& section_id);
    static rem::code init(std::function<rem::code()> inifn = nullptr);
    static rem::code append_section(const std::string& section_id);
    static rem::code end();
    //static rem::code endl();

    class OOM_API exception :  public std::exception
    {
    public:

        explicit exception():std::exception() { _e_ = true; }
        explicit exception(const char *txt) { _msg_ = txt; _e_ = false; }
        explicit exception(const std::string &txt) { _msg_ = txt; _e_ = false; }

        ~exception() noexcept override {
            _msg_.clear();
        }

        const char* what() const noexcept override;

        log::exception operator[](log& el);
    private:
        bool _e_;
        mutable std::string _msg_;
        log *_log_instance_{nullptr};
    };

    log() = default;
    log(rem::type message, rem::code code, std::source_location src);
    [[nodiscard]] rem::type entry_type() const { return _type_; }
};


}

//#endif //log_H
