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
#include <tuxvision/oomio/iobloc.h>

#include <utility>


namespace tux::io
{
class OOM_API ipoll
{
    CLASSNAME_(ipoll)

    iobloc::list _blocs_{};
    pollfd* _fds_{nullptr};


    // Sous reserve
    tux::action_delegator<tux::io::iobloc&> _polling_started_{};
    tux::action_delegator<tux::io::iobloc&> _polling_ended_{};
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

    void update_fd_state(iobloc& _fd, rem::action _action);
    void reset_pollfds();
    ipoll() = default;
    explicit ipoll(std::string  _id):_id_(std::move(_id)){}
    ~ipoll();

    iobloc::list& blocs(){ return _blocs_; }
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

    iobloc& add_fd(int fd=-1);
    rem::code remove_fd(int _fd_id);
};



}
