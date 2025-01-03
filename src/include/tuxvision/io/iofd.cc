//
// Created by oldlonecoder on 27/12/24.
//

#include <tuxvision/io/iofd.h>
#include <sys/ioctl.h>

namespace tux::io
{

iofd::iofd(int _fd, u8 _options, integers::u16 _poll_bits,  u64 _bfs): _fd_(_fd), _opt_(_options), _bloc_size_(_bfs),_poll_bits_(_poll_bits)
{
    _input_ready_signal_._id_ = "iofd::input_signal for FD=" + std::to_string(_fd_);
    _output_ready_signal_._id_ = "iofd::output_signal for FD=" + std::to_string(_fd_);
    reset();
}

std::string::iterator iofd::operator ++(int)
{
    auto c = _cursor_;
    if(_cursor_ < _buffer_.end())  ++_cursor_;
    return c;
}

std::string::iterator iofd::operator --()
{
    if(_cursor_ > _buffer_.begin())  --_cursor_;
    return _cursor_;
}

std::string::iterator iofd::operator ++()
{
    if(_cursor_ < _buffer_.end())  ++_cursor_;
    return _cursor_;
}

std::string::iterator iofd::operator --(int)
{
    auto c = _cursor_;
    if(_cursor_ > _buffer_.begin())  --_cursor_;
    return c;
}


iofd &iofd::operator <<(std::string _str)
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



char& iofd::operator *()
{
    return *_cursor_;
}

void iofd::reset()
{
    clear();
    _buffer_.resize(_bloc_size_+1);
    std::fill_n(_buffer_.begin(), _bloc_size_,static_cast<integers::u8>(0));
    _cursor_ = _buffer_.begin();
}


bool iofd::full()
{
    return (_cursor_ - _buffer_.begin()) >= _bloc_size_;
}

bool iofd::empty()
{
    return _buffer_.empty() || (_cursor_ - _buffer_.begin() == 0);
}

void iofd::clear()
{
    _buffer_.clear();
}


rem::code iofd::pause()
{
    _flags_.active = 0;
    _flags_.pause = 1;
    return rem::code::ok;
}


rem::code iofd::resume()
{
    _flags_.active = 1;
    _flags_.pause = _flags_.del = 0;
    return rem::code::ok;
}


rem::code iofd::remove()
{
    _flags_.active = 0;
    _flags_.pause = 0;
    _flags_.del = 1;
    return rem::code::ok;
}


rem::code iofd::init()
{
    _buffer_ = std::string(_bloc_size_,'\0');
    _cursor_ = _buffer_.begin();

    return rem::code::done;
}



iofd& iofd::set_poll_bits(u16 _bits)
{
    _poll_bits_ = _bits;
    return *this;
}



void iofd::terminate()
{
    remove();
}




/*!
 * @brief private called from polling class when have activity on this descriptor
 * @return rem::action::continu or rem::action::leave.
 *
 * @note descriptor::_buffer_ is not circular ...yet. It is a one-shot-reset on every calls
 */
rem::action iofd::input_()
{
    _buffer_.clear();
    u64 count = 0;
    char buffer[1024] = {0};
    ioctl(_fd_,FIONREAD, &count);
    if(!count)
    {
        _buffer_.clear();
        log::message() << " triggering descriptor on zero-byte signal..." << log::eol;
        return rem::action::end;
    }
    log::message() << " triggered descriptor handle #" << color::yellow << _fd_ << color::reset << ": ioctl FIONREAD reports:" << color::lightsteelblue3 << count << log::eol;
    //_buffer_.reserve(count);
    size_t bytes = ::read(_fd_, buffer, count);
    if(bytes != count)
    {
        log::error() << "poll_in read error: {" << color::lightsteelblue3 << tux::string::bytes(_buffer_) << log::eol;
        _buffer_.clear();
        return rem::action::leave;
    }

    _buffer_ = buffer;
    log::debug() << "poll_in read bytes: " << bytes << " / buffer.length() : " << _buffer_.length() << ": -> {" << tux::string::bytes(_buffer_) << "}" <<  log::eol;
    if(!_input_ready_signal_.empty())
    {
        return _input_ready_signal_(*this);
        //int loop_count = 0;
        //@todo Loop until all bytes in the buffer are eaten. - Implement circular buffer in case 1024 bytes isn't enough...Or discard remaining unhandled bytes as extraneous.
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


rem::action iofd::output_()
{
    return rem::action::continu;
}






}// namespace tux::ui::io
