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

#include <tuxvision/delegator.h>
#include <memory>


namespace tux::cmd
{


struct TUXVISION_API cmd_switch
{
    using data = std::vector<std::string_view>;
    using shared = std::shared_ptr<cmd_switch>;
    using array = std::vector<cmd_switch::shared>;
    using iterator = cmd_switch::array::iterator;

// ----------  ------------------------------------------
    std::string            name;
    std::string            switch_char;
    std::string            switch_text;
    std::string            descriptions;
    int                    required { 0 };
    int                    count    { 0 };
//--------------------------------------------------------


    data                   arguments{};
    bool                   enabled{ false };
    action_delegator<cmd_switch&> delegate_call_back;
    bool operator !() { return delegate_call_back.empty(); }

    template<typename T> void connect(T* Obj, rem::action (T::*Fn)(cmd_switch&)){
        delegate_call_back.connect(Obj, Fn);
    }
    cmd_switch& set_descriptions(const std::string& Txt);
    cmd_switch& set_char(const std::string& Txt);
    cmd_switch& set_long_switch(const std::string& Txt);
    cmd_switch& set_required(int N);

    ~cmd_switch();
};


class TUXVISION_API line
{

    cmd_switch::array arguments;
    cmd_switch defaults;

    cmd_switch::iterator A{};
public:
    line() = default;
    ~line();

    template<typename T> void set_default_callback(T* Obj, rem::code (T::*Fn)(cmd_switch&))
    {
        (void)defaults.delegate_call_back.connect(Obj, Fn);
    }

    cmd_switch::iterator query(std::string_view cmd_switch);
    cmd_switch& operator<<(const cmd_switch& Arg);
    cmd_switch& operator [] (const std::string& ArgName);
    //log::Enums::Code InputCmdLineData(int argc, char** argv);
    rem::code input(const std::vector<std::string_view>& StrArray);
    cmd_switch& add_cmd(const std::string& cId);
    rem::action process();
    bool empty() { return arguments.empty(); }
    std::string usage();
};

}
