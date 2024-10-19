
// Created by oldlonecoder on 8/13/24.
//

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


#include <csignal>
#include <map>
//#include <vector>
#include <string_view>
#include <tuxvision/tools/string.h>


using namespace integers;

namespace tux::globals
{


/**
 * @brief Widget ui justification enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace justify
{
using Type = U8;
static constexpr U8 left        = 0x01; ///< Justify left
static constexpr U8 right       = 0x02; ///< Justify right
static constexpr U8 hcenter     = 0x04; ///< Justify align horizontally at the center
static constexpr U8 vcenter     = 0x08; ///< Justify Justify align vertically at the center
static constexpr U8 center      = 0x0C; ///< Justify Vertical and Horizontal center
static constexpr U8 word_wrap   = 0x10; ///< Justify word wrap
static constexpr U8 prefix      = 0x20; ///< Justify prefix
static constexpr U8 suffix      = 0x40; ///< Justify suffix
static constexpr U8 auto_stretch = 0x80; ///< Justify auto-stretch
}


/**
 * @brief Ditux::ui::rectangleion enums values
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace direction
{
using type = I8;
static constexpr I8 left = -1; ///< to left
static constexpr I8 right = 1; ///< to right
static constexpr I8 up = 2;    ///< up
static constexpr I8 down = -2; ///< down
}

/**
 * @brief Widget ui class or'ed flags enums
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace wflags
{
using Type = U16;
static constexpr Type Unset = 0x0000;    ///< unset
static constexpr Type TopLevel = 0x0001; ///< Object is a top level widget
static constexpr Type Floating = 0x0002; ///< Floating widget ( and top level)
static constexpr Type Child = 0x0004;    ///< Object/widget is a child widget
static constexpr Type Parent = 0x0008;   ///< widget or object hasd a parent object
//...
}


namespace uistyle
{
using Type = U16;
static constexpr Type Unset     = 0x0000; ///< Unset
static constexpr Type Frame     = 0x0001; ///< Widget has a frame surrounding it (within geometry)
static constexpr Type Caption   = 0x0002; ///< Widget has a Caption label
static constexpr Type Input     = 0x0040; ///< Widget has or is an Input Field
static constexpr Type MultiLine = 0x0080; ///< Multiline input field
static constexpr Type Glyph     = 0x0100; ///< Glyph flag. Widget or widget's children components.
static constexpr Type statusbar = 0x0200; ///< Is a statusbar and/or has a statusbar
static constexpr Type hscroll   = 0x0400; ///< horizontally scrollable
static constexpr Type vscroll   = 0x0800; ///< vertically scrollable
}
/**
 * @brief Widget ui state enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace wstate
{
using Type = U16;
static constexpr Type Normal = 0x00;
static constexpr Type Disable = 0x01;
static constexpr Type Active = 0x02;
static constexpr Type Focus = 0x04;
static constexpr Type Error = 0x08;
static constexpr Type Warning = 0x10;
static constexpr Type Illegal = 0x20;
static constexpr Type Invalid = 0x40;
static constexpr Type Visible = 0x80;
static constexpr Type Success = 0x100;
static constexpr Type Done = 0x200;
static constexpr Type Ditry = 0x400;
}


/**
 * geometry policies for stretching and positionning children widgets
 */
namespace anchor
{
using value = U16;
static constexpr anchor::value fixed        = 0x0000; ///< free fixed xy placement
static constexpr anchor::value width_fit    = 0x0001; ///< Fits the entire width size
static constexpr anchor::value height_fit   = 0x0002; ///< Fits the entire height size
static constexpr anchor::value onframe_fit  = 0x0004; ///< Overlaps if wh size > 1, or sits on the frame.
static constexpr anchor::value fit_right    = 0x0008;
static constexpr anchor::value fit_left     = 0x0010;
static constexpr anchor::value fit_top      = 0x0020;
static constexpr anchor::value fit_bottom   = 0x0040;
static constexpr anchor::value fit_center   = 0x0080; ///< Both fit_h&vcenter ...
static constexpr anchor::value fit_hcenter  = 0x0100;
static constexpr anchor::value fit_vcenter  = 0x0200;

//...
}

namespace colors
{

/*!
 * \brief The attr_db class
 * Databse set of the colors. Theme, components, then state
 */
struct attr_db
{
    using components = std::map<wstate::Type,   color::pair>;
    using elements = std::map<std::string_view, attr_db::components>;
    using themes = std::map<std::string_view,   attr_db::elements>;
    //using theme     = std::map<std::string_view, db::dictionary>;
    static colors::attr_db::themes s_data;

    static attr_db::themes::iterator get_theme(std::string_view theme_name);
    static bool end(attr_db::themes::iterator it);
    //static attr_db::components& components_table(attr_db::themes::iterator i);
    static themes& theme();
};
} // namespace colors;

} // lus::intui::globals

