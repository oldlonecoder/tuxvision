
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

#include <tuxvision/object.h>
#include <tuxvision/tui/tuxns.h>
#include <tuxvision/tui/terminal.h>
#include <tuxvision/tui/vchar.h>
#include <tuxvision/tui/events.h>
#include <list>
namespace tux::ui
{


namespace terminal
{
class screen;
}


class OOM_API widget : public object
{
    CLASSNAME(widget)

    terminal::vchar::back_buffer _back_buffer_{nullptr};

    friend class terminal::screen;

public:
    widget();
    widget(object* _parent_obj, const std::string& _id);

    ~widget() override;


    rem::code set_geometry(const rectangle& r);
    rem::code set_theme(const std::string& theme_id);
    std::string theme_name(){ return _theme_id_; };

    rem::code                         peek_xy(ui::cxy xy);
    terminal::vchar::string::iterator at(ui::cxy xy);
    terminal::vchar::string::iterator operator[](ui::cxy xy);
    virtual widget&                   operator*() { return *this; }
    rem::code                         set_anchor(ui::anchor::value _ank);
    virtual rem::code                 show(ui::uistate::Type st);
    virtual rem::code                 show();
    virtual rem::code                 hide();

    virtual rem::code process_event(event& ev);

    // --------- State modifiers --------------------------------------------------------
    virtual void activate();

    virtual void set_focus();
    bool is_visible();
    [[nodiscard]] bool is_active() const;
    [[nodiscard]] bool is_focus() const;
    [[nodiscard]] bool is_hidden() const;
    [[nodiscard]] bool is_window() const { return _uistyle_& ui::uistyle::Window; }


    class OOM_API painter_dc
    {
        rectangle _geometry_{};
        widget* _widget_{nullptr};
        color::pair _colors_{};
        terminal::vchar::string::iterator _iterator_{};
        friend class widget;
        friend class screen;
        friend class window;
        //...
    public:
        painter_dc() = default;
        ~painter_dc() = default;
        painter_dc(widget* parent_widget, const rectangle& r);
        rem::code at(ui::cxy xy);
        rem::code sync();
        painter_dc& home();
        painter_dc& clear(const rectangle& r={});
        painter_dc& operator << (ui::cxy new_xy);
        painter_dc& operator << (char ch);
        painter_dc& operator << (const char* str);
        painter_dc& operator << (const std::string& str);
        painter_dc& operator << (tux::string str);
        painter_dc& operator << (color::code fgcol);
        painter_dc& operator << (color::pair colp);
        painter_dc& operator << (cadre::index ci);
        painter_dc& operator << (glyph::type ic);
        painter_dc& operator << (accent_fr::type fr);
        painter_dc& operator << (terminal::vchar vch);
        painter_dc& operator << (terminal::vchar::string strvch);
        painter_dc& set_background_color(color::code bgcol);
        painter_dc& operator*() { return *this; }
        rem::code operator ++();
        rem::code operator +=(size_t _offset);
        widget::painter_dc& draw_frame(cadre::frame_matrix fmatx={2,2,2,2,0});
    };

    virtual rem::code draw();
    widget::painter_dc begin_draw(const rectangle& sub_area={});
    void end_draw(widget::painter_dc& edc);
    void clear();
    virtual rem::code update();

/*
    static constexpr u16 None      = 0;
    static constexpr u16 Caption   = 0x0001; ///< Caption widgets
    static constexpr u16 Glyph     = 0x0002; ///< Glyph widgets
    static constexpr u16 StatusBar = 0x0004; ///< Status bar
    static constexpr u16 HScrollBar= 0x0008; ///< horizontal scrollbar
    static constexpr u16 VScrollBar= 0x0010; ///< vertical scrollbar
    static constexpr u16 Frame     = 0x0020; ///< Widget has a frame surrounding it (within geometry)
    static constexpr u16 MenuBar   = 0x0040; ///< Widget has a MenuBar system

 */
    bool has_frame();
    bool has_caption();
    bool has_glyph();
    bool has_status_bar();
    bool has_horizontal_scroll_bar();
    bool has_vertical_scroll_bar();
    bool has_menu_bar();
    bool has_component(ui::uistyle::Type b);
    bool has_style(ui::uistyle::Type b);
    bool has_status(ui::uistate::Type b);
    bool has_class(ui::uiclass::Type b);

    rem::code anchor_widget(widget* w);

    virtual rem::code setup_ui();
    virtual rem::code add_component(ui::components::type _cmp);

    void set_components(ui::components::type c);
    void set_uistyle(ui::uistyle::Type s);
    void set_uiclass(ui::uiclass::Type cls);
protected:
    ui::size _size_limits_{0,0, ui::cxy{1,1},ui::cxy{0xFFFF,0xFFFF}};
    rectangle _geometry_{}; ///< Dimensions, internal cursor position
    rectangle _dirty_area_{};



    std::string _theme_id_{"Default"};
    // --------- UI style and State----------------
    color::pair                          _colors_{};
    ui::colors::attr_db::components _states_colors_{}; ///< Widget States colors database.
    ui::colors::attr_db::elements   _elements_{};  ///< Widget colors elements database
    ui::anchor::value               _anchor_       {ui::anchor::None};     ///< AutoFit values for widgets placements within the parent 's widgets.
    ui::uistate::Type               _uistate_      {ui::uistate::Active};  ///< ...widgets state...{ visible, active,focus, etc ... }
    ui::uistyle::Type               _uistyle_      {ui::uistyle::None};    ///< Toplevel Window or child widgets components
    ui::uiclass::Type               _uiclass_      {ui::uiclass::None};    ///< css-like named class property of this widgets ( ex. : Error,Warning,Fatal,Information, etc... )
    ui::components::type            _uicomponents_ {ui::components::None}; ///< Sub/child-widgets components


    // --------------------------------------------

    virtual rem::code auto_fit();
    virtual rem::code resize(ui::size new_sz);
    virtual rem::code draw_frame(const rectangle& r);
    virtual rem::code dirty(const rectangle& dirty_rect);
    terminal::vchar::string::iterator _iterator_{}; ///< Normal std::vector iterator available for navigation and io.
    rem::code update_child(widget* w);
    rem::code render();
    rem::code position_child(widget* w);
    virtual rem::code setup_components();

public:
    void set_minimum_size(int w, int h){_size_limits_.min_size = ui::cxy{w,h};}
    void set_maximum_size(int w, int h){_size_limits_.max_size = ui::cxy{w,h};}
    ui::rectangle geometry() { return _geometry_; };
private:

    friend class screen;


};




} // namespace tux::ui
