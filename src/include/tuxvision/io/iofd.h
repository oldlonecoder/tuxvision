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



using namespace tux::integers;

namespace tux::io
{



class TUXVISION_API iofd final
{

    i64 _bloc_size_{-1};
    std::string _buffer_{};
    std::string::iterator _cursor_{};
    u16  _poll_bits_{0};
    int _fd_{-1};
    u8 _opt_{0};

    friend class ipoll;

public:


    static constexpr u8 IMM    = 1;
    static constexpr u8 NOBLOCK= 2;
    static constexpr u8 READ   = 4;
    static constexpr u8 WRITE  = 8;
    static constexpr u8 EXEC   = 0x10;


    using list   = std::vector<iofd>;
    using array  = iofd::list;

    iofd() = default;
    iofd(const iofd&) = default;
    iofd(iofd&&) noexcept = default;

    iofd(int _fd, integers::u8 _options, integers::u16 _poll_bits, integers::u64 _bfs);

    iofd& operator=(const iofd&) = default;
    iofd& operator=(iofd&&)noexcept = default;

    ~iofd() = default;
    rem::code init();
    iofd&     set_poll_bits(u16 _bits);
    void      terminate();
    // -----------------------------------------------------------------------------------------

    [[nodiscard]] std::string buffer() const { return _buffer_; }
    template<typename T> integers::i16 set_input_ready_delegate(T* _c_, rem::action(T::*_fn_ptr)(iofd&))
    {
        _input_ready_signal_._id_ = "iofd::input_signal for FD=" + std::to_string(_fd_);
        return _input_ready_signal_.connect(_c_, _fn_ptr);
    }

    template<typename T> integers::i16 set_output_ready_delegate(T* _c_, rem::action(T::*_fn_ptr)(iofd&))
    {
        _output_ready_signal_._id_ = "iofd::output_signal for FD=" + std::to_string(_fd_);
        return _output_ready_signal_.connect(_c_, _fn_ptr);
    }

    std::string::iterator operator ++();
    std::string::iterator operator ++(int);
    std::string::iterator operator --();
    std::string::iterator operator --(int);
    iofd& operator << (std::string _str);
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
        u8 active :1; ///< This iofd is active and monitored. - If not active then it is not monitored...Obviously ;)
        u8 del    :1; ///< This iofd is inactive , set to be removed and deleted.
        u8 pause  :1; ///< This iofd is set to pause, stopping its polling activity until resumed by setting its active bit on.
    }_flags_ = {0,0,0};
    
    rem::action input_();
    rem::action output_();


    action_delegator<iofd&>  _input_ready_signal_{"iofd::input_signal for FD=-1"};
    action_delegator<iofd&>  _output_ready_signal_{"iofd::input_signal for FD=-1"};

};



}
