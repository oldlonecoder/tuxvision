//
// Created by oldlonecoder on 24-06-14.
//

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


//#pragma once


#include <tuxvision/io/sscan.h>


namespace tux
{

sscan sscan::numeric::empty{};




sscan::sscan(std::string_view Txt)
{
    m_begin = m_pos = Txt.begin()+0;
    m_end = (m_begin + Txt.length())-1; // m_end placed at the last valid character. It will NEVER be changed.
    m_location = {1, 1, 0};

}


bool sscan::pop_location()
{
    if(m_points.empty()) return false;
    auto P = --m_points.end();
    m_location.line = P->first;
    m_location.col = P->second;
    m_points.erase(P);
    return true;
}


void sscan::push_location()
{
    m_points.emplace_back(m_location.line, m_location.col);

}

bool sscan::eof(const char* cc)
{
    return cc > m_end;
}


bool sscan::eof()
{
    return m_pos > m_end;
}


bool sscan::skip_ws()
{
    while (!eof() && std::isspace(*m_pos)) ++m_pos;
    return eof();
}

/*!
 * @brief Synchronize internal "cursor" location {line #, Column #, Offset} at the current iterator offset into the view.
 * @return  Reference to the updated infos into the location_data member of Processing.
 */
sscan::location_data &sscan::sync()
{
    auto c = m_begin;
    m_location.line = m_location.col = 1; //duh!!!!!!!
    while (!eof(c) && (c < m_pos)) {
        switch (*c) {
            case '\n':
                //rem::debug() << " new line :";
                ++c;
                if (*c == '\r') ++c;
                ++m_location.line;
                m_location.col = 1;
                continue;
            case '\r':
                //rem::debug() << " new line :";
                ++c;
                if (*c == '\n') ++c;
                ++m_location.line;
                m_location.col = 1;
                continue;
            case '\t':
            case '\v' :
                //throw rem::exception()[rem::error() << rem::codeurn::rejected << " - invalid character, intentionally not handled in this context."];
                //rem::error() << rem::code::rejected << "unhandled character.";

            default:
                ++c;
                ++m_location.col;
        }
    }
    m_location.offset = m_pos - m_begin;
    // ...
    //rem::debug() << mark();
    return m_location;
}






sscan::operator bool() const
{
    return m_begin != nullptr;
}


/*!
 * @brief Initiate this Processing with a (new) string_view.
 * @param view
 * @return reference to this instance.
 */
sscan &sscan::operator=(std::string_view view)
{

    if(view.empty())
    {
        //rem::error() << "assigned with an empty string \"view\". be aware, this instance cannot be used.";
        return *this;
    }
    m_begin = m_pos = view.begin()+0;
    m_end = m_begin + view.length()-1;
    m_location = {1, 1, 0};
    return *this;
}



bool sscan::operator++()
{
    if(eof()) return false;

    ++m_pos;
    //skip_ws();
    return !eof();
}

bool sscan::operator++(int)
{
    if(eof()) return false;

    ++m_pos;
    //skip_ws();
    return !eof();
}

rem::code sscan::seek(int32_t Idx)
{
//    if(text.empty()) return rem::code::Rejected;

//    Signal Exception on any attempt to seek into an empty string_view !! :
    if(m_begin && m_end) {

        // Otherwise just reject under/overflow :
        // Otherwise execute the seek and return Accepted.
        if (Idx < 0L) {
            size_t X = -1 * Idx;
            if ((m_end - X) < m_begin) X = 0;

            m_pos = m_end - X;
            sync();
            return rem::code::accepted;
        }
        if (((m_begin + Idx) < m_end)) m_pos = m_begin + Idx;
        else
            m_pos = m_end; // Jump to the end of the text...

        sync();
    }
    //else
        //Rem::Exception() [ Rem::Except() << rem::code::empty << ": Attempt to use the seek_at method on an un-assigned/incompletely assigned sscan!" ];
        //rem::error() << rem::code::empty << ": attempt to use the seek_at method on an un-assigned/incompletely assigned sscan!";
    
    return eof() ? rem::code::eof : rem::code::accepted;
}


sscan::numeric::result sscan::scan_number()
{
    numeric scanner(*this);
    if (!scanner.base2() && !scanner.base8() && !scanner.base16() && !scanner.base10()) return { rem::code::rejected, {} };

    return { rem::code::accepted, scanner.num_details };
}


/*!
 * @brief scan literal string between quotes
 * @return string_view of the contents including the surrounding quotes.
 */
std::pair<rem::code, std::string_view> sscan::scan_literal_string()
{
    tux::string Buf;
    std::string_view::iterator A = m_pos;


    if ((*A != '\'') && (*A != '"')) return { rem::code::rejected,{} };
    auto Q = *A++;

    // Ignore `""`  (empty string)
//loop_str:
    do {

        if (*A == '\\')
        {
            //rem::debug() << " a on '" << color::yellow << *A << color::reset << "':";
            Buf << *A++;
            //rem::debug() << " a on '" << color::yellow << *A << color::reset << "':";
            Buf << *A++;
            //rem::debug() << " a on '" << color::yellow << *A << color::reset << "':";
        }
        else
        {
            if (!eof())
                Buf << *A++;
        }
        if (*A == '"')
            Buf << *A;
    } while (!eof() && (*A != Q));

    if(eof())
        return { rem::code::rejected,{} };

    return { rem::code::accepted, {m_pos, ++A} };
}


std::string sscan::mark()
{
    // 1 - Locate Beginning of the line:
    tux::string Str;
    auto LBegin = m_pos;
    auto LEnd   = m_pos;
    // Beginning of the line:
    while((LBegin > m_begin) && (*LBegin != '\n') && (*LBegin != '\r')) --LBegin;
    if(LBegin < m_begin) LBegin = m_begin;
    // m_end of line:
    while((LEnd < m_end) && (*LEnd != '\n') && (*LEnd != '\r')) ++LEnd;
    if(LEnd > m_end) LEnd = m_end;
    auto col = (m_pos-LBegin); // ?
    auto spc = std::string(col-1<=0? 0 : col,' ');
    //rem::Debug() << ":---> nbspace: " << spc.length() << ":";
    Str | '\n' | std::string(LBegin, LEnd+1) | '\n' | spc | glyph::c_arrow_up;
    return Str();
}




/*!
 * @brief const char* at the beginning of the text.
 * @return string_view::iterator ( const char* );
 */
sscan::iterator sscan::begin()
{
    return m_begin; /// << The start if this Scanner
}


/*!
 * @brief const char* at the end of the text.
 * @return string_view::iterator ( const char* );
 */
sscan::iterator sscan::end()
{
    return m_end; //< the end of this scanner.
}

void sscan::push()
{
    p_stack.push(m_pos);
}

bool sscan::pop()
{
    if(p_stack.empty()) return false;
    m_pos = p_stack.top();
    p_stack.pop();
    return true;
}

rem::code sscan::reposition(std::size_t Offset)
{
    if(eof()) return rem::code::rejected;
    m_pos += Offset;
    return rem::code::accepted;
}

sscan::iterator sscan::start_sequence()
{
    push();
    return m_pos;
}

std::pair<sscan::iterator,sscan::iterator> sscan::end_sequence()
{
    auto I = p_stack.top();
    p_stack.pop();
    return {I, m_pos};
}

std::pair<sscan::iterator, sscan::iterator> sscan::scan(const std::function<rem::code()>& ScannerFn)
{
    start_sequence();
    if(!!ScannerFn())
        return end_sequence();
    auto I = end_sequence();
    back(I.first);
    return {};
}



/*!
 * @brief Scans for the first occurrence of \seq from the current position.
 * @param seq
 * @return Accepted if found, Rejected if not.
 * @todo Add more result codes to rem::Enum::Code !
 */
rem::code sscan::seek_at(const std::string_view &seq, int32_t a_pos)
{

    sync();
    auto sv= std::string_view(m_begin, (m_end-m_begin)+1);
    auto Start = a_pos > -1 ? a_pos : m_pos-m_begin; // Fuck!!!!
    auto pos = sv.find(seq, Start);

    if(pos == std::string_view::npos)
        return rem::code::rejected;
    m_pos = m_begin+pos;
    sync();
    return rem::code::accepted;
}

rem::code sscan::step(int32_t Idx)
{
    m_pos += Idx;
    if(m_pos > m_end)
        return rem::code::eof;
    return rem::code::accepted;
}

std::pair<rem::code, std::string_view> sscan::scan_identifier()
{
    auto cursor = m_pos;
    if(! std::isalpha(*cursor) && (*cursor != '_'))
        return {rem::code::rejected,{}};

    ++cursor;
    while(!eof() && (std::isalnum(*cursor) || (*cursor == '_'))) ++cursor;
    if(cursor > m_pos)
        return {rem::code::accepted, {m_pos, static_cast<uint32_t >(cursor-m_pos)}};

    return {rem::code::rejected,{}};

}


sscan::location_data const& sscan::location_data::operator>>(std::string &Out) const
{
    tux::string OutStr="%d,%d";
    OutStr << line << col;//  << Offset;
    Out = OutStr();
    return *this;
}

sscan::numeric::numeric(sscan &Tx):text(Tx), m_end(Tx.m_end), m_pos(Tx.m_pos), m_begin(Tx.m_pos){}

rem::code sscan::numeric::operator()()
{
    return rem::code::ok;
}

rem::code sscan::numeric::base2()
{
    // 0b11010101010101010101010101010101010                --> Parsed all
    // 0b1101'0101'01010101'''''''0101'0101'0101'0101'0100'1010  --> Parsed all
    // 0b1101 0101 0101 0101 0101 0101 0101 0101 0100'1010  --> Parsed : 0b1101
    // 11010101'01010101'01010101'01010101'01001010B        --> Parsed : Rejected
    // 11010101 01010101 01010101 01010101 01001010         --> Parsed :  [base 10] 11'010'101

    //rem::debug() << rem::fn::func;
    auto a = text();
    tux::string buf;
    num_details.base = details::base_size::Binary;

    if(std::toupper(*a) == 'b')
        buf << *a++;
    else
    {
        if(*a == '0')
        {
            buf << *a++;
            if (std::toupper(*a) == 'b')
                buf << *a++;
            else
            {
                //rem::write() << " base 2 : rejected @prefix '" << color::yellow << *a << color::reset << "'.";
                return rem::code::rejected;
            }
        }
    }
    if(a == m_begin)
    {
        //rem::write() << " base 2: rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }

    loop2:
    //rem::debug() << "loop2: a on '" << color::yellow << *a << color::reset << '\'';
    if(*a == '\'')++a;
    if(!isdigit(*a))
    {
        //rem::write() << " base 2: rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }
    while(!text.eof() && std::isdigit(*a))
    {
        if(*a>='2')
        {
            //rem::write() << " base 2: rejected @prefix '" << color::yellow << *a << color::reset << "'.";
            return rem::code::rejected;
        }
        buf << *a++;
    }
    if(!text.eof() && *a =='\'')
    {
        ++a;
        goto loop2;
    }

    if((a==m_begin) || (buf.empty())) return rem::code::rejected;// duh!!!
    //rem::debug() << " base2 : " << color::yellow << buf << rem::fn::endl << " length: " << color::lightcoral << seq.length();
    num_details.seq = {m_begin, a};
    num_details.value = std::stoi(buf(), nullptr, 2);
    num_details.base = details::base_size::Binary;
    num_details.scale_value();
    return rem::code::accepted;
}

rem::code sscan::numeric::base8()
{
    //rem::debug() << rem::fn::func;
    auto a = text();
    tux::string buf;
    num_details.base = details::base_size::Octal;

    std::string_view prefixes = "ooq@&";

    if(prefixes.find(*a) != std::string_view::npos )
        a++;
    else
    {
        //rem::write() << " base 8 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }

    loop8:
    //Rem::Debug() << "loop8: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    while(!text.eof() && std::isdigit(*a) && (*a <= '7'))
    {
        buf << *a++;
        //rem::debug() << " pushed digit'" <<  color::yellow << *(a-1) << color::reset << "' into the buffer. next:'" << color::yellow << *a << color::reset << "'";
    }
    if(!text.eof() && (*a == '\''))
    {
        ++a;
        goto loop8;
    }
    //rem::debug() << "octal loop exit: a on '" << color::yellow << *a << color::reset << '\'';

    if(((std::isdigit(*a) && (*a >= '7'))) || (a==m_begin))
    {
        //rem::write() << " base 8 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }
    //--a;
    //rem::debug() << "base8: a on '" << color::yellow << *a << color::reset << "' - buffer: [" << color::yellow << buf << color::reset << "]";
    if(buf.empty()) return rem::code::rejected;
    num_details.seq = {m_begin, a};
    num_details.value = std::stoi(buf(), nullptr, 8);
    num_details.base = details::base_size::Octal;
    num_details.scale_value();
    return rem::code::accepted;

}

rem::code sscan::numeric::base10()
{
    //rem::debug() << " base 2,8,16 rejected then:";
    auto a = text(); // get the current iterrator value...
    num_details.base = details::base_size::Decimal;

    tux::string buf;
    //if((*a == '.') || (*a == ','))
    if(*a == '.')
    {
        real = true;
        buf << '.'; // force '.' for convertion using tux::string >> ;
        ++a;
    }
    //rem::debug() << "decimal : a on '" << color::yellow << *a << color::reset << '\'';
    if(*a == '\'') ++a;
    //rem::debug() << "decimal : a on '" << color::yellow << *a << color::reset << '\'';
    if(!std::isdigit(*a) ){
        //rem::write() << " base 10 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }
    //rem::debug() << "a on '" << color::yellow << *a << color::reset << '\'';

    while(!text.eof() && std::isdigit(*a))
    {
        //rem::debug() << "while loop: a on '" << color::yellow << *a << color::reset << '\'';
        buf << *a++;
        //rem::debug() << "next to digit: a on '" << color::yellow << *a << color::reset << '\'';
        if(*a == '\''){++a; continue; }
        if((*a == '.'))// || (*a == ','))
        {
            if(real)
            {
                //rem::status() << " rejected on '" << color::yellow << *a << color::reset << "' - real flag already set.";
                return rem::code::rejected;
            }
            real = true;
            buf << '.';
            ++a;
            continue;
        }
        //rem::debug() << "bottom while loop: a on '" << color::yellow << *a << color::reset << '\'';
    }
    //rem::debug() << "a on '" << color::yellow << *a << color::reset << '\'';
    if(a == m_begin)
    {
        //rem::status() << " base 10: rejected on '" << color::yellow << *a << color::reset << "' sequence is not a number.";
        return rem::code::rejected;
    }

    num_details.seq = {m_begin, a};
    num_details.base = details::base_size::Decimal;

    buf >> num_details.value;
    if(!real)
        num_details.scale_value();
    else
        num_details.size = details::size_type::F32;

    //rem::debug() << " base10 : " << color::yellow << buf << rem::fn::endl << " length: " << color::lightcoral << seq.length();

    return rem::code::accepted;
}

rem::code sscan::numeric::base16()
{
    //rem::debug() << " base 2,8 rejected - then:";
    auto a = text();
    tux::string buf;
    num_details.base = details::base_size::Hexadecimal;
    // do not wanna add '$'
    if (*a == '0')
    {
        ++a;

        if(std::toupper(*a) != 'x')
        {
            //rem::write() << " base 16 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
            return rem::code::rejected;
        }
    }
    else
        if(*a !='$') // hard coded
            return rem::code::rejected;
    ++a;
    //rem::write() << " hex prefix validated, now begin to scan : on char [" << color::yellow << *a << color::reset << "':";
    loop16:
    //rem::debug() << "loop16: a on '" << color::yellow << *a << color::reset << '\'';
    while(!text.eof() && std::isxdigit(*a)) buf << *a++;
    if(!text.eof() && (*a == '\'')){ ++a; goto loop16; }
    if(a==m_begin) return rem::code::rejected;
    //rem::debug() << rem::fn::func << "buffer contents '" << color::yellow << buf() << color::reset << '\'';
    num_details.seq = {m_begin, a};
//    Rem::Debug() << "[Hex] : Sequence ["
//    << Color::Yellow << std::string(num_details.seq.begin(), num_details.seq.end())
//    << Color::Reset << "] Length: "
//    << Color::LightCoral << num_details.seq.length()
//    << Color::White <<  " -> Buffer:["
//    << Color::Yellow << Buf
//    << Color::Reset << "] ";
    if(buf.empty()) return rem::code::rejected;
    num_details.value = std::stoi(buf(), nullptr, 16);
    num_details.scale_value();
    return rem::code::accepted;
}

void sscan::numeric::sign()
{

}

void sscan::numeric::details::scale_value()
{


    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 8bits:
    if((value >=-128) && (value <=127))
    {
        size = size_type::I8;
        return;
    }
    if((value >= 128) && (value <=255))
    {
        size = size_type::U8;
        return;
    }
    if((value >= -32768) && (value <= 32767))
    {
        size = size_type::I16;
        return;
    }
    if((value >=32768) && (value <= 65565))
    {
        size = size_type::U16;
        return;
    }
    if((value >= -2147483648) && (value <= 2147483647))
    {
        size = size_type::I32;
        return;
    }
    if((value >= 0x10000) && (value <= 4294967295))
    {
        size = size_type::U32;
        return;
    }
    if((value < 0LL))
    {
        size = size_type::I64;
    }
    else
        size = size_type::U64;

}
} // CC
