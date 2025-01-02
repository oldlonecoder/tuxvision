//
// Created by oldlonecoder on 27/12/24.
//

#include <tuxvision/oomio/iobloc.h>


namespace tux::io
{

iobloc::iobloc(int _fd, u64 _options, u16 _bfs): _fd_(_fd), _opt_(_options), _bloc_size_(_bfs)
{
    _input_ready_signal_._id_ = "iobloc::input_signal for FD=" + std::to_string(_fd_);
    _output_ready_signal_._id_ = "iobloc::output_signal for FD=" + std::to_string(_fd_);
    reset();
}

std::string::iterator iobloc::operator ++(int)
{
    auto c = _cursor_;
    if(_cursor_ < _buffer_.end())  ++_cursor_;
    return c;
}

std::string::iterator iobloc::operator --()
{
    if(_cursor_ > _buffer_.begin())  --_cursor_;
    return _cursor_;
}

std::string::iterator iobloc::operator ++()
{
    if(_cursor_ < _buffer_.end())  ++_cursor_;
    return _cursor_;
}

std::string::iterator iobloc::operator --(int)
{
    auto c = _cursor_;
    if(_cursor_ > _buffer_.begin())  --_cursor_;
    return c;
}


iobloc &iobloc::operator <<(std::string _str)
{
    auto c = _str.begin();
    for(;_cursor_ < _buffer_.end(); ++_cursor_) *_cursor_ = *c++;
    if(full())
    {
        _input_ready_signal_(*this);
        clear();
    }
    //...
    return *this;
}



char& iobloc::operator *()
{
    return *_cursor_;
}

void iobloc::reset()
{
    clear();
    _buffer_.resize(_bloc_size_+1);
    std::fill_n(_buffer_.begin(), _bloc_size_,static_cast<integers::u8>(0));
    _cursor_ = _buffer_.begin();
}

bool iobloc::full()
{
    return (_cursor_ - _buffer_.begin()) >= _bloc_size_;
}

bool iobloc::empty()
{
    return _buffer_.empty() || (_cursor_ - _buffer_.begin() == 0);
}

void iobloc::clear()
{
    _buffer_.clear();
}


#pragma region _descriptor_


rem::code iobloc::init()
{
    _buffer_ = std::string(_bloc_size_,'\0');
    _cursor_ = _buffer_.begin();
    return rem::code::done;
}








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






}// namespace tux::ui::io
