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



#include <sys/ioctl.h>
#include <tuxvision/oomio/ipoll.h>


namespace tux::io
{


#pragma region _descriptor_

// iobloc::~iobloc()
// {
//     _buffer_.clear(); // Really needed? Is the _buffer_ 's destructor isn't clearing its own allocated heap ?
// }



iobloc& iobloc::set_poll_bits(u16 _bits)
{
    _config_.poll_bits = _bits;
    return *this;
}


bool iobloc::operator++()
{
    if (_config_.cursor >= _buffer_.end())
    {
        _config_.cursor  = _buffer_.end();
        return false;
    }
    ++_config_.cursor;
    return _config_.cursor != _buffer_.end();
}


bool iobloc::operator++(int)
{
    if (_config_.cursor >= _buffer_.end())
    {
        _config_.cursor  = _buffer_.end();
        return false;
    }
    ++_config_.cursor;
    return _config_.cursor != _buffer_.end();
}


bool iobloc::operator--()
{
    if (_config_.cursor == _buffer_.end())
    {
        _config_.cursor  = _buffer_.end();
        return false;
    }
    --_config_.cursor;
    return _config_.cursor >= _buffer_.begin();
}

void iobloc::terminate()
{

}


bool iobloc::operator--(int)
{
    if (_config_.cursor == _buffer_.end())
    {
        _config_.cursor  = _buffer_.end();
        return false;
    }
    --_config_.cursor;
    return _config_.cursor >= _buffer_.begin();
}


/*!
 * @brief private called from polling class when have activity on this descriptor
 * @return rem::action::continu or rem::action::leave.
 *
 * @note descriptor::_buffer_ is not circular ...yet. It is a one-shot-reset on every calls
 */
rem::action iobloc::poll_in()
{
    _buffer_.clear();
    u64 count = 0;
    char buffer[1024] = {0};
    ioctl(_config_.fd,FIONREAD, &count);
    if(!count)
    {
        _buffer_.clear();
        log::message() << " triggering descriptor on zero-byte signal..." << log::eol;
        return rem::action::end;
    }
    log::message() << " triggered descriptor handle #" << color::yellow << _config_.fd << color::reset << ": ioctl FIONREAD reports:" << color::lightsteelblue3 << count << log::eol;
    //_buffer_.reserve(count);
    size_t bytes = ::read(_config_.fd, buffer, count);
    if(bytes != count)
    {
        log::error() << "poll_in read error: {" << color::lightsteelblue3 << tux::string::bytes(_buffer_) << log::eol;
        _buffer_.clear();
        return rem::action::leave;
    }

    _buffer_ = buffer;
    log::debug() << "poll_in read bytes: " << bytes << " / buffer.length() : " << _buffer_.length() << ": -> {" << tux::string::bytes(_buffer_) << "}" <<  log::eol;
    if(!_in.empty())
    {
        return _in(*this);
        //int loop_count = 0;
        //@todo Loop until all bytes in the buffer are eaten. - Implement circular buffer in case if 1024 bytes isn't enough...Or discard remaining unhandled bytes as extraneous.
        // while (loop_count < 10)
        // {
        //     if (auto a = _in(*this); a==rem::action::leave || a==rem::action::end)
        //     {
        //         //...
        //         log::status() << a << " on the " << loop_count << "'th iteration." << log::eol;
        //         return a;
        //     }
        //     ++loop_count;
        // }
    }

    // Handled or not, we clear the buffer here before returning for the next event.
    _buffer_.clear();
    return rem::action::continu;
}


rem::action iobloc::poll_out()
{
    return rem::action::continu;
}




/*!
 * @brief Create a new shared bare tux::io::descriptor object
 * @return the reference of the new descriptor config_data to be filled with its proper configs
 */
iobloc::config_data& polling::new_descriptor()
{
    log::info() << " adding a new descriptor - the file handle integer will be set." << log::eol;
    _descriptors_.emplace_back(std::make_shared<iobloc>());
    return _descriptors_.back()->_config_;
}

iobloc &polling::add_descriptor(int _fd)
{
    log::info() <<  " adding a new descriptor - the file handle must still be set." << log::endl;
    _descriptors_.emplace_back(std::make_shared<iobloc>());
    auto fd = _descriptors_.back();
    if(_fd>=0)
        fd->config().fd = _fd;
    return *fd;
}


/*!
 * @brief remove descriptor object identified by its fd number, if found in the _descriptors_ array.
 * @param _fd_id
 * @return
 */
rem::code polling::remove_descriptor(int _fd_id)
{
    auto count = _descriptors_.size();
    for(auto& s: _descriptors_)
        if(s->_config_.fd == _fd_id)
            _descriptors_.erase(std::ranges::find(_descriptors_, s));

    _state_ = _descriptors_.empty() ? state::Terminate : state::Run;
    return count == _descriptors_.size() ? rem::code::rejected : rem::code::accepted;
}


#pragma endregion _descriptor_

#pragma region _polling_

void polling::update_descriptor_state(iobloc& d, rem::action _action)
{
    switch(_action)
    {
        case rem::action::continu:
            break;
        case rem::action::leave:
        case rem::action::end:
            remove_descriptor(d.config().fd);
            break;
        default:break;
    }
}


/*!
 * @brief after all modifiations on the _fds_ pool, rebuild the array.
 */
void polling::reset_pollfds()
{
    log::debug() << " (re)-building modified descriptors list or initial polling setup before first run:" << log::eol;
    delete [] _fds_;
    _fds_ = new pollfd[_descriptors_.size()];
    memset(_fds_, 0, sizeof(pollfd) * _descriptors_.size());
    for(int i=0; i<_descriptors_.size(); ++i)
    {
        _fds_[i].fd = _descriptors_[i]->config().fd;
        _fds_[i].events = _descriptors_[i]->config().poll_bits;

    }
}


polling::~polling()
{
    delete [] _fds_;
}


rem::code polling::start_polling()
{
    log::info() << " starting polling..." << log::eol;
    reset_pollfds();

    return run();
}


/*!
 * @brief Starts the loop thread
 * @return rem::code
 */
rem::code polling::run()
{
    log::info() << " polling loop starting..." << log::eol;
    _state_ = Run;
    while(_state_ != state::Terminate)
    {
        log::debug() << " looping..." << log::eol;
        auto r = poll(_fds_, _descriptors_.size(), -1);
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
            log::debug() << "revents: " << _fds_[f].revents << log::eol;
            if(_fds_[f].revents & POLLIN)
            {
                if(_descriptors_[f]->_flags_.pause || !_descriptors_[f]->_flags_.active)
                {
                    log::status() << "descriptor index #" << f << "is inactive" << log::eol;
                    continue;
                }

                if(auto a = _descriptors_[f]->poll_in(); a != rem::action::continu)
                    update_descriptor_state(*_descriptors_[f],a);
                else
                    _fds_[f].events = _descriptors_[f]->config().poll_bits; // Pas si s&ucirc;r qu'on ait besoin de faire &ccedil;a tout le temps &agrave; chaque cycle...
                continue;
            }
            if(_fds_[f].revents & POLLHUP)
            {
                // todo signal hangup event.
                //_state_ = state::Terminate;
                if(!remove_descriptor(_descriptors_[f]->config().fd))
                    log::warning() << "Descriptor " << _descriptors_[f]->config().fd << " not removed, not in this polling list...(?)" << log::eol;
                update_descriptor_state(*_descriptors_[f], rem::action::end);
            } // continue naturally until I add code below ...
        }
        // Write is not yet needed.
    }
    log::debug() << " polling loop done, state=" << static_cast<int>(_state_) << log::eol;
    return rem::code::done;
}

rem::code polling::set_state(polling::state _state)
{
    _state = _state;
    return rem::code::accepted;
}

void polling::terminate()
{
    _state_ = state::Terminate;
}

#pragma endregion _polling_

}// namespace tux::ui::io;
