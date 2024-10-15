//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include "tuxvision/exports"
#include <tuxvision/ui/globals.h>
#include <tuxvision/journal/book.h>
#include <tuxvision/tools/signals.h>
#include <tuxvision/tools/object.h>

namespace tux::ui::terminal
{


rectangle geometry();



book::code query_winch();

//static terminal& instance();
//terminal();

book::code begin();
book::code end();


void switch_alternate();
void switch_back();

void start_mouse();
void stop_mouse();

void clear();
void cursor_off();
void cursor_on();
void cursor(ui::cxy xy);
void mv(globals::direction::type dir=globals::direction::right, int d=1);

enum caret_shapes: u8
{
    def,
    bloc_blink,
    block_steady,
    under_blink,
    under_steady,
    vbar_blink,
    vbar_steady
};

enum class DECMode
{
    line_wrap               = 7,
    crs_shape               = 25,
    mouse_X10               = 9,
    mouse_vt200             = 1000,
    mouse_vt200_hl          = 1001,
    mouse_report_buttons    = 1002,
    mouse_report_any        = 1003,
    mouse_utf8              = 1005,
    mouse_sgr_ext_mode      = 1006,
    mouse_urxvt_mode        = 1015,
    mouse_sgr_pixel_mode    = 1016,
    alternate_screen        = 1049
};

constexpr const char* CSI = "\x1b[";
signals::action<rectangle>& term_resize_signal();


struct _TUXVISION_ vchar final
{
    u32 d{0x8003A020};
    using string = std::vector<vchar>;
    using back_buffer = std::shared_ptr<terminal::vchar::string>;


    struct _TUXVISION_ bloc final
    {
        ui::size                geometry{};
        terminal::vchar::string  buffer{};

        bloc copy(const ui::rectangle& _area={}); ///< Extraction
        //book::code copy_object(const ui::rectangle& _sub_area, object* from);
        book::code pasta(const vchar::bloc& src, ui::cxy _to); ///< Insertion

    };

    static constexpr u32 CharMask  = 0x800000FF;
    static constexpr u32 FGMask    = 0x0000FF00;
    static constexpr u32 BGMask    = 0x00FF0000;
    static constexpr u32 CMask     = 0x00FFFF00;
    static constexpr u32 ColorsMask= 0x00FFFF00;
    static constexpr u32 AttrMask  = 0xFF000000;
    static constexpr u32 UtfMask   = 0xB10000FF;
    static constexpr u32 StyleMask = 0x0E000000;
    // ==============u===attributes==========================
    static constexpr u32 UGlyph    = 0x01000000;
    static constexpr u32 Underline = 0x02000000;
    static constexpr u32 Stroke    = 0x04000000;
    static constexpr u32 Blink     = 0x08000000;
    static constexpr u32 Accent    = 0x10000000;
    static constexpr u32 Frame     = 0x20000000;
    static constexpr u32 TRGB      = 0x40000000;
    /// This will maku use of true-color RGB command instead of the re-routed terminal GUI colors profile
    static constexpr u32 ASCII     = 0x80000000;
    static constexpr u32 UTFBITS   = Accent | Frame | UGlyph;
    // ==============Bit Shift ============================
    static constexpr int FGShift = 0x08;
    static constexpr int BGShift = 0x10;
    static constexpr int ATShift = 0x18;



    vchar() = default;

    explicit vchar(U32 Ch);
    explicit vchar(vchar* C);
    vchar(vchar& C){d = C.d;}
    vchar(const vchar& C){d = C.d;}
    vchar(color::pair cc);
    ~vchar() = default;

    vchar& set_fg(color::code fg_);
    vchar& set_bg(color::code bg_);

    vchar& set_attributes(U32 Ch);
    //vchar& set_colors(color::pair&&Ch);
    // vchar& ResetAttributes(vchar::Type bits_);

    vchar& set_colors(const color::pair&CP);

    vchar& operator=(U32 Ch);
    vchar& operator=(const vchar& Ch) = default;
    vchar& operator=(const U32* Ch);
    vchar& operator=(char Ch);
    vchar& operator=(int Ch)
    {
        d = Ch;
        return *this;
    };

    [[nodiscard]] std::string get_utf_string() const;

    [[nodiscard]] color::code     foreground() const;
    [[nodiscard]] color::code     background() const;
    [[nodiscard]] color::pair     colors() const;
    [[nodiscard]] glyph::type     icon_id() const;
    [[nodiscard]] accent_fr::type accent_id() const;
    [[nodiscard]] cadre::index    frame_id() const;
    [[nodiscard]] char     ascii() const;
    vchar&  operator*() { return *this; }
    [[nodiscard]] uint16_t attributes() const;
    vchar& operator <<(glyph::type gl);
    vchar& operator <<(accent_fr::type ac);
    vchar& operator << (cadre::index fr);
    vchar& operator <<(color::pair cp);


    vchar& operator <<(vchar Ch)
    {
        d = Ch.d;
        return *this;
    }

    static std::string render(const vchar::string& _string);
    static std::string render(const vchar* _blk, int _width);
    static book::code render_string(vchar::string::iterator start, vchar::string::iterator end);



    vchar&     operator <<(char Ch);
    [[nodiscard]] std::string render_colors() const;
    explicit    operator std::string() const;
    [[nodiscard]] std::string details() const;
};

}
