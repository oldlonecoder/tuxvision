//
// Created by oldlonecoder on 14/06/24.
//
//  matricule UPS:
#include <tuxvision/journal/logger.h>


/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge tuxsier                                          *
 *   serge.tuxsier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge tuxsier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge tuxsier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge tuxsier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/


//#pragma once
namespace Fs = std::filesystem;


namespace tux
{
//std::vector<log> log::logs{};
std::mutex log_guard{};

log::section::list log::sections{};
log::section::list::iterator log::current_section{};




log &log::error(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::err, rem::code::failed, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::warning(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::warning, rem::code::ok, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::fatal(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::fatal, rem::code::failed, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::except(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::except, rem::code::rejected, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::message(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::message, rem::code::implemented, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::write(std::source_location src)
{

    log_guard.lock();
    auto& r = **log::current_section << log{};
    r.text | "\r\n";
    log_guard.unlock();
    return r;
    // log_guard.lock();
    // if(log::sections.empty())
    // {
    //     abort();
    //     auto& r = **log::current_section << log{rem::type::warning, rem::code::empty, src};
    //     r << " using uninitialized log storage -  Initializing with default: 'globals'";
    //     r << "\r\n";
    //     log_guard.unlock();
    //     return r;
    // }
    //
    // if(!log::current_section->contents.empty())
    // {
    //     //@todo apply encoded newline : either ascii or html
    //     auto& le = log::current_section->contents.back();
    //     le.text | "\r\n"; // this way we make sure that we set new line for all oses.
    //     log_guard.unlock();
    //     return le;
    // }
    // auto& r = **log::current_section << log{rem::type::output, rem::code::empty, src};
    // log_guard.unlock();
    // return r;
}

log &log::debug(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::debug, rem::code::ok, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::info(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::info, rem::code::implemented, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::comment(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::comment, rem::code::implemented, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::syntax(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::syntax, rem::code::rejected, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::status(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::status, rem::code::implemented, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::test(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::test, rem::code::implemented, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::interrupted(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::interrupted, rem::code::rejected, src};
    r.init_header();
    log_guard.unlock();
    return r;
}

log &log::aborted(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::aborted, rem::code::rejected, src};
    r.init_header();
    log_guard.unlock();
    return r;
}


log &log::segfault(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::segfault, rem::code::blocked, src};
    r.init_header();
    log_guard.unlock();
    return r;
}


log& log::jnl(std::source_location src)
{
    log_guard.lock();
    auto& r = **log::current_section << log{rem::type::log, rem::code::blocked, src};
    r.init_header();
    log_guard.unlock();
    return r;

}


log &log::operator<<(const std::string &txt)
{
    text << txt;
    return *this;
}

log &log::operator<<(const char *txt)
{
    text << txt;
    return *this;
}

log &log::operator<<(std::string_view txt)
{
    text << txt;
    return *this;
}

log &log::operator<<(const rectangle &r)
{
    text << r.tostring();
    return *this;
}

log &log::operator<<(cxy pt)
{
    text | std::format("{},{}", pt.x, pt.y);
    return *this;
}

log &log::operator<<(color::code col)
{
    text | col;
    return *this;
}

log &log::operator<<(color::pair fgbg)
{
    text | fgbg();
    return *this;
}

log &log::operator<<(glyph::type gh)
{
    text | gh;
    return *this;
}

log &log::operator<<(rem::type ty)
{
    auto [gh,colors] = rem::type_attributes(ty);
    text | colors | gh  | rem::to_string(ty) | color::reset;
    return *this;
}


log &log::operator<<(accent_fr::type ac)
{
    text | accent_fr::data[ac];
    return *this;
}

void log::init_header()
{
    if(_headercomp_.stamp)
    {
        auto txt{tux::string::now("%H:%M:%S")};
        auto [ic, a] = rem::function_attributes(rem::fn::stamp);
        tux::string acc;
        acc | a.fg | glyph::data[ic]  | txt;
        text | acc() | " ";
    }
    if(_headercomp_.type)
    {
        auto [gh,colors] = rem::type_attributes(_type_);
        text | colors | gh  | ' ' | rem::to_string(_type_) | color::reset | ' ';
    }

    if(_headercomp_.file)
    {
        auto [gh, colors] = rem::function_attributes(rem::fn::file);
        tux::string txt = location.file_name();
        tux::string::word::list words;
        if(auto count = txt.words(words, false, "/"); !count) return ;
        text | color::reset | " in " | colors | gh;
        if(words.size() > 1)
        {
            auto i = words.end();
            --i; // filename
            auto filename = *i;
            --i; // parent dir
            text | **i;
            ++i;
            text | '/' | **i | ' ';
        }
        words.clear();
    }
    // disable uncontrollable and potentially insane length of function signature given by std::source_location.
    // instead, using it explicitly into a distinct log input
    //------------------------------------------------------------------------------------------------------------
    //    if(headercomp.fun)
    //    {
    //        auto [gh, colors] = log::functionattributes(rem::fn::fun);
    //        text << colors << gh << ' ' << location.function_name() << color::reset;
    //    }
    //------------------------------------------------------------------------------------------------------------
    if(_headercomp_.line)
    {
        auto [gh, colors] = rem::function_attributes(rem::fn::line);
        text | colors | "line: " | std::format("{}", location.line()) | color::reset;
    }
    if(_headercomp_.fun)
        (*this) << rem::fn::endl << rem::fn::func;

    //text | " \r\n";
}


log &log::operator<<(char ch)
{
    text | ch;
    return *this;
}

void log::purge(const std::function<void(log &)>& f)
{
    std::cout << " Purge log::sections->contents:\n\r";
    for(auto& sec: log::sections)
    {
        std::cout << " Section: [" << sec.id << "]:\n\r--------------------------------------------------------\n\r";
        for(auto& r : sec.contents)
        {
            if (f)
                f(r);
            else
                std::cout << r.text() << "\r\n";

            r.text.clear();
        }
        sec.contents.clear();
    }
}


log &log::operator<<(rem::action a_action)
{
    auto [gh,colors] = rem::action_attributes(a_action);
    switch(a_action)
    {
        case rem::action::commit :
        {
            if(!log::current_section->fout)
                return *this;

            *(log::current_section->fout) << text() << "\r\n" << std::flush;
            log::current_section->remove_last();
            return *this;

        }
        break;
        default:
            text | colors() | gh | rem::to_string(a_action);
        break;
    }

    return *this;
}


log& log::operator<<(const tux::string::list& _list)
{
    text |  tux::string::make_str(_list);
    //...
    return *this;
}


log& log::operator<<(log::oef e)
{
    log::current_section->contents.back() << color::reset << rem::action::commit;
    return *this;
}


log &log::operator<<(rem::code cod)
{
    auto [gh,colors] = rem::return_code_attributes(cod);
    text
        | colors()
        | gh
        | rem::to_string(cod)
        | color::reset | ' ';
    return *this;
}

log &log::operator<<(rem::fn fn)
{
    switch (fn) {
        case rem::fn::endl:
            text | '\n';
//            switch (applog::format()) ///@todo create log::format(); directly instead.
//            {
//                case color::format::ansi256:
//                    input("\n");
//                    break;
//                case color::format::html:
//                    input("<br />");
//                    break;
//            }
            break;
        case rem::fn::stamp: {
            /*
                 * %d %f ou %d %m %y
                 * %r %t %h %m %s
                 * %a {monday} %a {mon}
                 * %b {june}   %b {jun}
            */


            //std::chrono::zoned_time date{"america/toronto", std::chrono::system_clock::now()};
            //const auto tp{std::chrono::system_clock::now()};
            //auto txt{tux::string::now("{:%h:%m:%s}", tp)};
            auto [ic, a] = rem::function_attributes(rem::fn::stamp);
            tux::string acc;
            acc | a.fg | glyph::data[ic] | color::reset |tux::string::now("%H:%M:%S");
            text | acc() | " ";
            return *this;
        }

        case rem::fn::file:
            text | location.file_name();
            return *this;
        case rem::fn::weekday: {
            auto [ic, a] = rem::function_attributes(rem::fn::weekday);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc | /*utf::glyph::data[ic] <<*/ a.fg | tux::string::now("%a");
            text | acc();
            return *this;
        }

        case rem::fn::day : {
            auto [ic, a] = rem::function_attributes(rem::fn::day);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc | /*utf::glyph::data[ic] <<*/ a.fg | tux::string::now("%d");
            text | acc();
            return *this;
        }

        case rem::fn::month: {
            auto [ic, a] = rem::function_attributes(rem::fn::month);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc | /*utf::glyph::data[ic] <<*/ a.fg | tux::string::now("%m");
            text | acc();
            return *this;
        }
        case rem::fn::monthnum: {
            auto [ic, a] = rem::function_attributes(rem::fn::month);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc | /*utf::glyph::data[ic] <<*/ a.fg | tux::string::now("%b");
            text | acc();
            return *this;
        }
        case rem::fn::year: {
            auto [ic, a] = rem::function_attributes(rem::fn::year);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc | /*utf::glyph::data[ic] <<*/ a.fg | tux::string::now("%y");
            text | acc();
            return *this;
        }
        case rem::fn::func:
            auto [gh, colors] = rem::function_attributes(rem::fn::func);
            text | "From " | colors | location.function_name() | color::reset | "\n";
            break;

        //default: break;
    }
    return *this;
}


log::exception log::exception::operator[](log& el)
{
    _log_instance_ = &el;
    return *this;
}


log::log(rem::type in_type, rem::code code, std::source_location src):_type_(in_type), _code_(code), location(std::move(src)){text | "\r\n"; }


const char* log::exception::what() const noexcept
{
    if(_log_instance_)
    {
        _msg_ = _log_instance_->text();
        return _msg_.c_str();
    }
    return std::exception::what();
}


log::section &log::select(const std::string& section_id)
{
    if(log::sections.empty())
    {
        log::init();
        log::current_section = log::sections.begin();
        return log::sections[0];
    }
    int count=0;
    for(auto& s : log::sections)
    {
        if(s.id == section_id)
        {
            log::current_section = log::sections.begin() + count;
            return s;
        }
        ++count;
    }
    log::current_section = log::sections.begin();
    return *log::current_section;
}


rem::code log::init(std::function<rem::code()> inifn)
{
    log::sections.emplace_back(log::section{.id="globals"});
    log::current_section = log::sections.begin();
    log::current_section->ofs.open("global.log", std::ios_base::trunc|std::ios_base::out);
    if(!log::current_section->ofs.is_open())
        throw "Could not open log.";

    log::current_section->fout = &log::current_section->ofs;
    if(inifn)
        return inifn();
    return rem::code::done;
}

rem::code log::append_section(const std::string &section_id)
{
    if(log::select(section_id).id == section_id)
    {
        log::current_section = log::sections.begin();
        log::error() << " section identified by '" << section_id << "' already exists. - request rejected";
        return rem::code::rejected;
    }
    log::sections.emplace_back(log::section{.id=section_id});
    log::current_section = --log::sections.end();
    std::string filename{};
    filename = section_id + ".log";
    log::current_section->ofs.open(filename.c_str(), std::ios_base::trunc|std::ios_base::out);
    log::current_section->fout = &log::current_section->ofs;

    //--log::current_section;
    return rem::code::accepted;
}


rem::code log::end()
{
    ///@todo Iterate sections and close their output file.
    for(auto& sec : log::sections)
    {
        sec.ofs.close();
        sec.contents.clear();
    }

    ///...
    //log::current_section->close();
    return rem::code::done;
}


// rem::code log::endl()
// {
//     log::current_section->contents.back() << rem::action::commit;
//     return rem::code::accepted;
// }


log::section &log::section::operator*()
{
    return *this;
}

log &log::section::operator<<(log &&r)
{
    contents.emplace_back(r);
    return contents.back();
}


void log::section::remove_last()
{
    if(!contents.empty()) contents.pop_back();
    //log::debug() << "section size: " << contents.size() << log::eol;
}


void log::section::close()
{
    log::info() << rem::fn::func << " " << rem::fn::month << "," << rem::fn::year << " " << rem::fn::hour << ":"  << rem::fn::minute << ":" << rem::fn::seconds << rem::action::end;
    if(ofs.is_open()) ofs.close();
}
} // namespace oom
