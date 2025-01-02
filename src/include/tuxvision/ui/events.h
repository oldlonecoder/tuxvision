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



#pragma once


#if defined(linux)
#   include <unistd.h>
//#   include <sys/epoll.h>
//#   include <fcntl.h>
#else
#    error "non-linux os not implemented yet"
#endif

#include <tuxvision/journal/logger.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/types.h>

//#include <stack>
//#include <deque>

namespace tux::ui
{
using namespace integers;




struct OOM_API key_event
{
        /**
     * @brief Key mnemonics.
     *
     */
    enum enums : U8
    {
        ArrowUP    ,
        ArrowDown  ,
        ArrowLeft  ,
        ArrowRight ,
        Enter      ,
        ESC        ,
        BS         ,
        Home       ,
        End        ,
        PgUp       ,
        PgDown     ,
        INS        ,
        TAB        ,
        DEL        ,
        F1         ,
        F2         ,
        F3          ,
        F4          ,
        F5          ,
        F6          ,
        F7          ,
        F8          ,
        F9          ,
        F10         ,
        F11         ,
        F12         ,
        ShiftF2     ,
        ShiftEnd    ,
        ShiftHome   ,
        ShiftArrowUp,
        ShiftArrowDown,
        ShiftArrowLeft,
        ShiftArrowRight,
//        ShiftF1,
//        ShiftF2,
        ShiftF3,
        ShiftF4,
        ShiftF5,
        ShiftF6,
        ShiftF7,
        ShiftF8,
        ShiftF9,
        ShiftF10,
        ShiftF11,
        ShiftF12,
        ShiftCtrlArrowLeft,
        ShiftCtrlArrowRight,
        ShiftCtrlArrowUp,
        ShiftCtrlArrowDown,
        CtrlArrowLeft,
        CtrlArrowRight,
        CtrlArrowUp,
        CtrlArrowDown,
        CtrlInsert,
        CtrlHome,
        CtrlQ,
        CtrlS,
        CtrlV,
        CtrlBS,
        ShiftTab,
        ShiftEnter,
        CtrlF8,
        ShiftCtrlDel,
        ShiftCtrlHome,
        ShiftAltArrowLeft,
        ShiftAltArrowRight,
        ShiftAltArrowUp,
        ShiftAltArrowDown,
        ShiftAltEnd,
        ShiftAltHome,
        ShiftAltPgUp,
        ShiftAltPgDown,
        ShiftPageUp,
        ShiftPageDown,
        CtrlPageUp,
        CtrlPageDown,
        CtrlDelete,
        CtrlEnd,
        AltLeftArrow,
        AltRightArrow,
        AltUpArrow,
        AltDownArrow,
        CtrlAltLeftArrow,
        CtrlAltRightArrow,
        CtrlAltUpArrow,
        CtrlAltDownArrow,
        ShiftCtrlPageUp,
        ShiftCtrlPageDown,
        AltInsert,

        //...

        None,

    };
    U64 code{0}; ///< enough room for each BYTE ex [SHIFT+F5] bytes sequence: 0x1b,0x5b,0x31,0x35,0x3b,0x32,0x7e
                 ///< is compressed into 0x00005B31353B327E.
                 ///< ...But we need to compact the bytes sequence into a u64 each time.

    std::string_view description;
    char ansi_seq[20]; // Null-terminated chars array - We keep storing the raw sequence in string of bytes
    key_event::enums  mnemonic{enums::None};
    using key_db = std::map<u64,key_event>;
    using key_group = std::vector<key_event>;
    using list      = key_event::key_group;
    static key_event::key_group command_keys_db;
    static key_event::key_group shifted_command_keys_db;
    static key_event::key_group ctrl_command_keys_db;
    static key_event nokey;
//    std::string_view operator[](key_event::enums) const; -- disabled. It was at the beginning when there was only a single keys group...
    //static const key_event& query(key_event::key_group& k_db, const char* Seq);
    //static const key_event& query(const char* Seq);
    static const key_event& query(U64 key_code);
    static const key_event& query(key_event::key_group& k_db, U64 key_code);
    std::string_view sequence();

};


struct OOM_API commandevent
{

};

struct OOM_API fileinputevent{};


// Special handling is required for HMTM which is not described in this document. In order to highlight terminals should support shift-mouseclick while mouse tracking is enabled for the user to copy and paste.
// xterm Mouse Tracking Flags
// Code	Effect	Note
// 0	MB1	Mouse Button 1 pressed
// 1	MB2	Mouse Button 2 pressed
// 2	MB3	Mouse Button 3 pressed
// 4	MB+S	Mouse Button + Shift
// 8	MB+A	Mouse Button + Alt
// 8	MB4	Mouse Button 4 pressed
// 10	MB5	Mouse Button 5 pressed
// 16	MB+C	Mouse Button + Ctrl
// 32	MBM	Mouse Button Motion
// 64	MWU	Mouse Wheel Up
// 65	MWD	Mouse Wheel Down
// 128	MB8	Mouse Button 8 pressed
struct OOM_API mouse
{


    struct
    {   // -- buttons: 0 = release; 1 = pressed - no matter previous and motion states
        u8 left   :1;
        u8 middle :1;
        u8 right  :1;
        u8 _4     :1;
        u8 _5     :1;
        u8 _6     :1;
    }button{0};

    // Modifiers:
    bool shift = false;
    bool meta = false;
    bool control = false;

    // Coordinates:
    bool move{false}; ///< Quick check for mouse motion ( delta > {0,0} )

    ui::cxy oldxy {};///< {0,0} or last position.
    ui::cxy xy{}; ///< move,delta := |xy-oldxy|
    ui::cxy dxy{}; ///< move,delta := |xy-oldxy|
    ui::cxy delta(); ///<
    std::string to_string();
};



struct  OOM_API event
{

    static rem::code get_stdin_event(event& _event_, timeval tv={0xffffffffff,0});
    static rem::code init();
    static mouse mouse_data;
    operator bool();

    enum class type : u8
    {
        key_input,
        key_command,
        mouse_move,
        mouse_button,
        UNCOMPLETED,
        DROP,
        CHARACTER,
        MOUSE,
        CURSOR_POSITION,
        CURSOR_SHAPE,
        SPECIAL,

        //...
        noop,
    }event_type{event::type::noop};

    union
    {
        key_event       kev;
        mouse           mev;
        commandevent    cmdev;
        fileinputevent  fev;
    }data{};

    using events_q = std::array<event,20>; //< I want to implement fixed length ( for speed ) circular events Q so std::vector is the one to choose over std::deque; std::dq; std::stack

    static constexpr u64 noevent                = 0x0000000000000000;
    static constexpr u64 mouse_left_down        = 0x0000000000000001;
    static constexpr u64 mouse_right_down       = 0x0000000000000002;
    static constexpr u64 mouse_middle_down      = 0x0000000000000004;
    static constexpr u64 mouse_left_release     = 0x0000000000000008;
    static constexpr u64 mouse_right_release    = 0x0000000000000010;
    static constexpr u64 mouse_middle_release   = 0x0000000000000020;
    static constexpr u64 mouse_move             = 0x0000000000000040;
    static constexpr u64 mouse_drag_enter       = 0x0000000000000080;
    static constexpr u64 mouse_drag_leave       = 0x0000000000000100;
    static constexpr u64 mouse_drag             = 0x0000000000000200;
    static constexpr u64 mouse_hover_enter      = 0x0000000000000400;
    static constexpr u64 mouse_hover_leave      = 0x0000000000000800;
    static constexpr u64 mouse_hover            = 0x0000000000001000;
    static constexpr u64 command_key            = 0x0000000000002000;
    static constexpr u64 character_key          = 0x0000000000004000;
    static constexpr u64 cancel                 = 0x0000000000008000;
    static constexpr u64 accept                 = 0x0000000000010000;
    static constexpr u64 quit                   = 0x0000000000020000;
    static constexpr u64 close                  = 0x0000000000040000;
    static constexpr u64 open                   = 0x0000000000080000;
    static constexpr u64 save                   = 0x0000000000100000;
    static constexpr u64 scroll                 = 0x0000000000200000;
    static constexpr u64 clear                  = 0x0000000000400000;

    //...
    static constexpr u64 extended_event         = 0x8000000000000000;
    u64 event_bits{event::noevent};
    static u64 prev_mouse_state;
    bool operator[](u64 event_code) const { return event_code & event_bits; }



};


class OOM_API events_stream final
{

    event::events_q _events_q{};

    event::events_q::iterator _head_{_events_q.begin()-1}; ///< iterator on the actual event instance and index into the Q
    event::events_q::iterator _tail_{_events_q.begin()}; ///< tail index on circular queue
    std::string _id_{"no name yet or none"};
public:
    events_stream() = default;
    events_stream(const std::string& _id);
    ~events_stream();

    event& push();
    rem::code get(event& _event);
    event& operator*() const {return *_head_;}

};

}
