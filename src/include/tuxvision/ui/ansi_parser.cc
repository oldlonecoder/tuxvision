#include <tuxvision/tui/io/ansi_parser.h>

#include <tuxvision/tui/terminal.h>



namespace tux::ui::io
{

    using ui::color;

ansi_parser::ansi_parser(iofd::shared _data): _fd_(_data){}


/*!
 * @brief starts or continue to parse data from the terminal STDIN_FILENO
 * @param event
 * @return
 */
rem::code ansi_parser::parse(event& ev)
{
    cursor = _fd_->config().cursor;
    if (cursor == _fd_->end()) return rem::code::eof;
    log::debug() << "parsing sequence: {" << color::lightsteelblue3 <<  tux::string::bytes(_fd_->buffer()) << color::reset << '}' << log::endl;
    if (*cursor == 0x1b)
    {
        if (_fd_->buffer().length()>1) return parse_ESC(ev);
        log::debug() << "escape character pressed: " << log::endl;
        ev.event_bits = event::cancel|event::command_key;
        ev.data.kev.code = key_event::enums::ESC;
        ev.data.kev.ansi_seq[0]=27;
        ev.data.kev.ansi_seq[1]=0;

        return rem::code::ready;
    }
    //---------------------------------------------------------------
    // Not ESC seq.
    // Check CTRL hit ( *cursor < ascii(32) ) + ENTER,TAB, BACKSP ...
    // One Byte key. All extra keys are ignored/discarded.

    if((*cursor < 32) || (*cursor == 127))
    {

        ev.event_type = event::type::key_command;
        log::write() << "KEY_COMMAND: " << color::yellow << (int)*cursor << color::reset << "';" << log::endl;

        //@todo Register ALL ctrl-KEY to the dictionary!
        if(key_event tmpkev = key_event::query(*cursor); tmpkev.mnemonic != key_event::None)
        {
            ev.data.kev=tmpkev;
            next_byte();
            return rem::code::ready;
        }
        ev.event_type = event::type::CHARACTER;
        ev.data.kev.code = static_cast<u64>(static_cast<unsigned char>(*cursor));
        next_byte();
        return rem::code::ready;
    }
    std::strncpy(ev.data.kev.ansi_seq, _fd_->buffer().c_str(), 19);
    ev.event_type = event::type::CHARACTER;
    ev.data.kev.code = static_cast<u8>(*cursor); // In this case the code member contains the ascii code of the character.

    return rem::code::ready;
}


rem::code ansi_parser::parse_ESC(event& ev)
{
    // next character:
    next_byte();
    log::debug() << " next byte: " << color::aquamarine3 << std::format("{:02x}",static_cast<u8>(*cursor)) << log::endl;
    if (cursor == _fd_->end()) return rem::code::eof;

    switch (*cursor)
    {
        case 'P':
            log::debug() <<  static_cast<int>(*cursor) << rem::code::notimplemented << log::endl;
            return rem::code::notimplemented;// parse_dcs();
        case '[':
            log::debug() << " calling parse_CSI: " << log::endl;
            return parse_csi(ev); ///@note Le focus est ici [ clefs de commandes et autres ], mouse...
        case ']':
            log::debug() <<  static_cast<int>(*cursor) << rem::code::notimplemented << log::endl;
            return rem::code::notimplemented;//parse_osc(evd);

        // Expecting 2 characters.
        case ' ':
        case '#':
        case '%':
        case '(':
        case ')':
        case '*':
        case '+':
        case 'O':
        case 'N': {
            if (!next_byte())
            {
                ev.event_type = event::type::UNCOMPLETED;
                log::debug() <<  static_cast<int>(*cursor) <<  rem::code::notimplemented << log::endl;
                return rem::code::notimplemented;
            }
            return parse_ss1_2(ev);
        }
        // Expecting 1 character:
        default:
            ev.event_type = event::type::SPECIAL;
            log::debug() <<  static_cast<int>(*cursor) << rem::code::notimplemented << log::endl;
            return rem::code::notimplemented;
        break;
    }
}


rem::code ansi_parser::parse_csi(event& ev)
{
    if (!next_byte()) return rem::code::eof;

    log::debug() << log::endl;

    bool altered = false;
    int argument = 0;
    std::vector<int> arguments;
    while (true) {
        if (!next_byte()) {
            log::write() << " End of the sequence: switch to parse_ss1_2:" << log::endl;
            return parse_ss1_2(ev);
        }

        if (*cursor == '<') {
            //log::write() << " altered mode";
            altered = true; // Just bypass ...for now...
            continue;
        }

        if (*cursor >= '0' && *cursor <= '9') {
           // log::write() << "cursor on digit: '" << color::yellow << (*cursor-'0') << color::reset << "'" << log::endl;
            argument *= 10;  // NOLINT
            argument += *cursor - '0';
            continue;
        }

        if (*cursor == ';') {
            log::write() << "arg separator: arg value: '" << color::yellow << argument << color::reset << "' " << log::endl;
            arguments.push_back(argument);
            argument = 0;
            continue;
        }

        // CSI is terminated by a character in the range 0x40–0x7E
        // (ASCII @A–Z[\]^_`a–z{|}~),
        // Note: I don't remember why we exclude '<'
        // To handle F1-F4, we exclude '['.
        if (*cursor >= '@' && *cursor <= '~' && *cursor != '<' && *cursor != '[')
        {
            log::write() << "CSI completed: last arg : '" << color::yellow << argument << color::reset << "' " << log::endl;
            arguments.push_back(argument);
            argument = 0;  // NOLINT
            int c=1;
            for(auto n : arguments) log::write() << c++ << " > " << std::format("0x{:02x}",n);
            switch (*cursor) {
                case 'M':
                    return parse_mouse(ev,std::move(arguments));
                case 'm':
                    return parse_mouse(ev, std::move(arguments));
                case 'R':
                    return rem::code::eof; //parse_caret_report(std::move(arguments));
                default:
                    log::write() << " Switching to parse_ss_1_2():" << log::endl;
                    return parse_ss1_2(ev);
            }
        }
        log::write() << " Unterminated CSI: switchting to parse_ss1_2" << log::endl;
        return parse_ss1_2(ev);
    }
}


rem::code ansi_parser::parse_mouse(event& ev, std::vector<int>&& args)
{
    // pressed 'flag' ignored. Relying on the XTerm Button and meta state byte which reports buttons on the lasts two bits:
    log::debug() << log::endl;

    if (args.size() != 3) return rem::code::eof; //parse_ss_1_2(evnt);
    ev.event_type = event::type::MOUSE;

    event::mouse_data.button = {0,0,0};
    event::mouse_data.button.left   = (args[0] & 3) == 0 ? 1 :0;
    event::mouse_data.button.middle = (args[0] & 3) == 1 ? 1 :0;
    event::mouse_data.button.right  = (args[0] & 3) == 2 ? 1 :0;
    ///@todo Handle the other buttons...
    /// ...

    event::mouse_data.shift     = args[0] & 4;
    event::mouse_data.meta      = args[0] & 8;
    event::mouse_data.control   = args[0] & 0x10;
    if(event::mouse_data.meta)
        log::info() << color::pair({.fg=color::grey100, .bg=color::red4}) << "meta" << log::endl;
    // Subtract 1 from the coords. Because the terminal starts at 1,1 and our ui system starts 0,0
    event::mouse_data.xy.x = args[1]-1;
    event::mouse_data.xy.y = args[2]-1;
    event::mouse_data.move = event::mouse_data.oldxy != event::mouse_data.xy;
    event::mouse_data.dxy = {event::mouse_data.xy.x-event::mouse_data.oldxy.x, event::mouse_data.xy.y-event::mouse_data.oldxy.y};
    event::mouse_data.oldxy = event::mouse_data.xy;
    ev.data.mev = event::mouse_data; ///< copy mouse state to the event data.
    log::info() << "Mouse data:" << ev.data.mev.to_string() << log::endl;
    return rem::code::ready;
}


rem::code ansi_parser::parse_ss1_2(event& ev)
{
    // 8 bytes max which include the beginning of the buffer (ESC;O | [)

    // Consume the buffer. key_event::ansi_seq is now the final location.
    log::debug() << log::endl;
    //log::write() << "copy seq into event kev ansi_seq: ";
    std::strncpy(ev.data.kev.ansi_seq, _fd_->buffer().c_str(), 19);

    auto& code = ev.data.kev.code;
    code = 0;
    auto ln = std::min(size_t(8),std::strlen(ev.data.kev.ansi_seq));
    //log::write() << " Check: ansi_seq to parse: {" << color::yellow << tuxin::string::bytes(evd.data.kev.ansi_seq) << color::reset << "}:";

    U8 _b_ = ev.data.kev.ansi_seq[0]; // start with ESC byte
    code = (code << 8) | _b_;
    _b_ = ev.data.kev.ansi_seq[1]; // no matter '[' or 'O' :
    code = (code << 8) | _b_;
    // Now assign the rest towards ln or until  hit or miss:
    for(int x=2;x < ln; x++)
    {
        _b_ = ev.data.kev.ansi_seq[x];
        code = (code << 8) | _b_;
        log::write() << std::format("{:04x}",code) << log::endl;
        if(key_event tmp_kev = key_event::query(code); tmp_kev.mnemonic != key_event::None)
        {
            log::write() << "Match:" << color::lime << tmp_kev.description << log::endl;
            ev.data.kev = tmp_kev;
            ev.event_type = event::type::key_command;
            return rem::code::ready;
        }
    }

    return rem::code::eof;

}


bool ansi_parser::next_byte()
{
    ++*_fd_;
    cursor = _fd_->config().cursor;
    return cursor != _fd_->end();
}



}
