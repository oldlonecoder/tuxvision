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

#include <tuxvision/journal/logger.h>
#include <tuxvision/delegator.h>
#include <arpa/inet.h>

namespace tux::io
{



class OOM_API iobloc final
{

    integers::i64 _bloc_size_{-1};
    std::string _buffer_{};
    std::string::iterator _cursor_{};
    int _fd_{-1};
    integers::u64 _opt_{0};
    friend class ipoll;

public:

    using list   = std::vector<iobloc>;
    using array  = iobloc::list;

    iobloc() = default;
    iobloc(const iobloc&) = default;
    iobloc(iobloc&&) noexcept = default;

    iobloc(int _fd, integers::u64 _options, integers::u16 _bfs);

    iobloc& operator=(const iobloc&) = default;
    iobloc& operator=(iobloc&&)noexcept = default;

    ~iobloc() = default;
    rem::code init();
    // -----------------------------------------------------------------------------------------

    [[nodiscard]] std::string buffer() const { return _buffer_; }
    template<typename T> integers::i16 set_input_ready_delegate(T* _c_, rem::action(T::*_fn_ptr)(iobloc&))
    {
        _input_ready_signal_._id_ = "iobloc::input_signal for FD=" + std::to_string(_fd_);
        return _input_ready_signal_.connect(_c_, _fn_ptr);
    }

    template<typename T> integers::i16 set_output_ready_delegate(T* _c_, rem::action(T::*_fn_ptr)(iobloc&))
    {
        _output_ready_signal_._id_ = "iobloc::output_signal for FD=" + std::to_string(_fd_);
        return _output_ready_signal_.connect(_c_, _fn_ptr);
    }

    std::string::iterator operator ++();
    std::string::iterator operator ++(int);
    std::string::iterator operator --();
    std::string::iterator operator --(int);
    iobloc& operator << (std::string _str);
    char& operator *();

    void reset();
    bool full();
    bool empty();
    void clear();

    template<typename T=integers::u8> T* at() { return static_cast<T*>(&(*_cursor_)); }

    std::string::iterator end() { return _cursor_ + (_cursor_ - _buffer_.begin()) + 1; }

    integers::i16 ntohs() const { return ::ntohs(static_cast<integers::i16>(*_cursor_)); }
    integers::i32 ntohl() const { return ::ntohl(static_cast<integers::i32>(*_cursor_)); }
    integers::i16 htons() const { return ::htons(static_cast<integers::i16>(*_cursor_)); }
    integers::i32 htonl() const { return ::htonl(static_cast<integers::i32>(*_cursor_)); }

    rem::code pause();
    rem::code resume();
    rem::code remove();
    
private:
    struct flags
    {
        u8 active :1; ///< This iobloc is active and monitored. - If not active then it is not monitored...Obviously ;)
        u8 del    :1; ///< This iobloc is inactive , set to be removed and deleted.
        u8 pause  :1; ///< This iobloc is set to pause, stopping its polling activity until resumed by setting its active bit on.
    }_flags_ = {0,0,0};
    
    rem::action input_();
    rem::action output_();


    action_delegator<iobloc&>  _input_ready_signal_{"iobloc::input_signal for FD=-1"};
    action_delegator<iobloc&>  _output_ready_signal_{"iobloc::input_signal for FD=-1"};

};



}
