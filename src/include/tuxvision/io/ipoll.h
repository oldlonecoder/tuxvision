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
#include <tuxvision/string.h>
#include <tuxvision/journal/logger.h>
#include <poll.h>
#include <tuxvision/io/iofd.h>

#include <utility>


namespace tux::io
{
class TUXVISION_API ipoll
{
    CLASSNAME_(ipoll)

    iofd::list _iofd_list_{};
    pollfd* _sys_fd_{nullptr};


    // Sous reserve
    tux::action_delegator<tux::io::iofd&> _polling_started_{};
    tux::action_delegator<tux::io::iofd&> _polling_ended_{};
    // --------------------------------------------------------------

public:
    enum state : u8
    {
        Unset,
        Run,
        Stop,
        Pause,
        Terminate
    }_state_{Unset};
private:

    std::string _id_{};
public:

    void update_fd_state(iofd& _fd, rem::action _action);
    void reset_fd_list();
    ipoll() = default;
    explicit ipoll(std::string  _id):_id_(std::move(_id)){}
    ~ipoll();

    iofd::list& fds(){ return _iofd_list_; }
    const std::string& id(){ return _id_; }
    rem::code start_polling();
    rem::code run();
    rem::code stop();
    rem::code pause();
    rem::code resume();
    rem::code reset();
    rem::code end();
    rem::code set_state(ipoll::state _state);
    void terminate();

    iofd&     add_fd(int _fd, u8 _poll_opt, u64 _poll_mask);
    rem::code remove_fd(int _fd_id);

};



}
