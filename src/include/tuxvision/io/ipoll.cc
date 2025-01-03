// Created by oldlonecoder on 8/13/24.
//

//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
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




#include <tuxvision/io/ipoll.h>


namespace tux::io
{






iofd &ipoll::add_fd(int _fd, u8 _poll_opt, u64 _poll_mask)
{
    log::info() <<  " adding a new descriptor - the file handle must still be set." << log::endl;
    _iofd_list_.emplace_back(_fd, _poll_opt, _poll_mask,1024);
    auto& fd = _iofd_list_.back();
    fd._input_ready_signal_._id_ = _id_ + std::to_string(_fd);
    return fd;
}


/*!
 * @brief remove descriptor object identified by its fd number, if found in the _descriptors_ array.
 * @param _fd_id
 * @return
 */
rem::code ipoll::remove_fd(int _fd_id)
{
    auto count = _iofd_list_.size();
    for(auto fit = _iofd_list_.begin(); fit != _iofd_list_.end(); ++fit)
        if(fit->_fd_ == _fd_id)
            _iofd_list_.erase(fit);

    _state_ = _iofd_list_.empty() ? state::Terminate : state::Run;
    return count == _iofd_list_.size() ? rem::code::rejected : rem::code::accepted;
}


void ipoll::update_fd_state(iofd& d, rem::action _action)
{
    switch(_action)
    {
        case rem::action::continu:
            break;
        case rem::action::leave:
        case rem::action::end:
            remove_fd(d._fd_);
            break;
        default:break;
    }
}


/*!
 * @brief after all modifiations on the _fds_ pool, rebuild the array.
 */
void ipoll::reset_fd_list()
{
    log::debug() << " (re)-building modified descriptors list or initial polling setup before first run:" << log::eol;
    delete [] _sys_fd_;
    _sys_fd_ = new pollfd[_iofd_list_.size()];
    memset(_sys_fd_, 0, sizeof(pollfd) * _iofd_list_.size());
    for(int i=0; i<_iofd_list_.size(); ++i)
    {
        _sys_fd_[i].fd = _iofd_list_[i]._fd_;
        _sys_fd_[i].events = _iofd_list_[i]._poll_bits_;

    }
}


ipoll::~ipoll()
{
    delete [] _sys_fd_;
}


rem::code ipoll::start_polling()
{
    log::info() << " starting polling..." << log::eol;
    reset_fd_list();
    return run();
}


/*!
 * @brief Starts the loop thread
 * @return rem::code
 */
rem::code ipoll::run()
{
    log::info() << " polling loop starting..." << log::eol;
    _state_ = Run;
    while(_state_ != state::Terminate)
    {
        log::debug() << " looping..." << log::eol;
        auto r = poll(_sys_fd_, _iofd_list_.size(), -1);
        log::debug() << " poll returned: " << r << log::eol;
        if(r == -1)
        {
            log::error() << "poll() :'" << color::red4 <<  std::strerror(errno) << color::reset << '\'' << log::eol;
            _state_ = state::Terminate;
            continue;
            // @todo Signal the error... doh!
        }
        if(!r)
        {
            log::status() << " timeout (lightly...) " << log::eol;
            continue;
        }
        for(int f = 0; f < r; f++)
        {
            log::debug() << "revents: " << _sys_fd_[f].revents << log::eol;
            if(_sys_fd_[f].revents & POLLIN)
            {
                if(_iofd_list_[f]._flags_.pause || !_iofd_list_[f]._flags_.active)
                {
                    log::status() << "descriptor index #" << f << "is inactive" << log::eol;
                    continue;
                }

                if(auto a = _iofd_list_[f].input_(); a != rem::action::continu)
                    update_fd_state(_iofd_list_[f],a);
                else
                    _sys_fd_[f].events = _iofd_list_[f]._poll_bits_; // Pas si s&ucirc;r qu'on ait besoin de faire &ccedil;a tout le temps &agrave; chaque cycle...
                continue;
            }
            if(_sys_fd_[f].revents & POLLHUP)
            {
                // todo signal hangup event.
                //_state_ = state::Terminate;
                if(!remove_fd(_iofd_list_[f]._fd_))
                    log::warning() << "Descriptor " << _iofd_list_[f]._fd_ << " not removed, not in this polling list...(?)" << log::eol;
                update_fd_state(_iofd_list_[f], rem::action::end);
            } // continue naturally until I add code below ...
        }
        // Write is not yet needed.
    }
    log::debug() << " polling loop done, state=" << static_cast<int>(_state_) << log::eol;
    return rem::code::done;
}

rem::code ipoll::set_state(ipoll::state _state)
{
    _state_ = _state;
    return rem::code::accepted;
}

void ipoll::terminate()
{
    _state_ = state::Terminate;
}

}// namespace tux::ui::io;
