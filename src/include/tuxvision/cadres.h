//
// Created by oldlonecoder on 24-04-02.
//

//#ifndef APPBOOK_CADRES_H
//#define APPBOOK_CADRES_H
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


#include <stack>
#include <tuxvision/colors.h>
#include <tuxvision/glyphes.h>

namespace tux
{

using namespace integers;
using ui::color;
struct OOM_API cadre
{
    color::pair colors;     ///< colours of the box::frame


/*!
 * @brief 1v x 4h matrix for addressing Frame components
 * @note  : Source : https://github.com/ArthurSonzogni/FTXUI/blob/main/src/ftxui/screen/string.cpp
 */
    struct OOM_API frame_matrix
    {
        uint8_t Left : 2;
        uint8_t Top  : 2;
        uint8_t Right: 2;
        uint8_t Down : 2;
        uint8_t Round: 1;

        bool operator < (cadre::frame_matrix RHS) const;
    };

    struct OOM_API frame_data
    {
        glyph::T TL; ///< Top Left
        glyph::T TR; ///< Top Right
        glyph::T BL; ///< Bottom Left
        glyph::T BR; ///< Bottom Right
        glyph::T HL; ///< Horizontal Line
        glyph::T VL; ///< Vertical Line
        glyph::T CVH;///< Cross Vertical-Horizontal
        glyph::T CRL;///< Cross Right-towards-Left
        glyph::T CLR;///< Cross Left-towards-Right
        glyph::T CTD;///< Cross Top-towards-Down
        glyph::T CDT;///< Cross Down-towards-Top
        cadre::frame_data& operator = (cadre::frame_matrix FrameModel);

    }frame{ "┏", "┓", "┗", "┛", "━", "┃", "╋", "┫", "┣", "┳", "┻" }; ///< By ...default, hardcode the ...default model.
/*           ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━{x,x,x,0,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━{x,0,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━{0,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━{x,x,0,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━{x,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,0,x,0}
             ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,x,0,0}
             ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,x,0,0,0}
             ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,x,0,0}
             ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,0,x,0}
             ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,0,x,x,0}

*/
    cadre();
    cadre::frame_matrix _model_{2,2,2,2,0}; ///< (default) Frame model to apply to frame_data.
    static std::map<std::string_view, cadre::frame_matrix> s_matrix_data;
    const char** data[11] = {&frame.TL, &frame.TR,&frame.BL,&frame.BR,&frame.HL,&frame.VL,&frame.CVH,&frame.CRL,&frame.CLR,&frame.CTD,&frame.CDT};
    enum index : int
    {
        TopLeft=0, TopRight, BottomLeft, BottomRight, Horizontal, Vertical, Cross, VerticalTowardsLeft, VerticalTowardsRight, TopTowardsDown, DownTowardsUp
    };

    static std::stack<cadre::frame_matrix> s_fm;
    static void push(cadre::frame_matrix FrameModel);
    static void pop();

    cadre& operator=(cadre::frame_matrix Model);
    // .Frame[Mem & CharMask]; Enum: Cadre::TopLeft
    std::string operator [](cadre::index Ix)
    {
        //const char* -> std::string(const char*); // Creates a deep copy and that is what we want.
        // if(Ix > DownTowardsUp)
        //     throw rem::exception() [rem::fatal() << "cadres::index oob (" << Ix << ")!"];
        return *data[Ix];
    }
};


}
