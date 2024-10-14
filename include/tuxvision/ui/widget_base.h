#ifndef WIDGET_BASE_H
#define WIDGET_BASE_H

#include <tuxvision/tools/object.h>
#include <tuxvision/ui/globals.h>
#include <tuxvision/ui/terminal.h>
namespace tux::ui
{


class _TUXVISION_ widget_base : public object
{
    CLASSNAME(widget_base)

    terminal::vchar::back_buffer _bloc_{nullptr};

protected:
    rectangle _geometry_{}; ///< Dimensions, internal cursor position
    rectangle _dirty_area_{};

    std::string _theme_id_{"Default"};
    // --------- UI style and State----------------
    color::pair                          _colors_{};
    globals::colors::attr_db::components _style_{};
    globals::colors::attr_db::elements   _theme_elements_{};
    globals::wstate::Type                _state_{globals::wstate::Active};
    globals::anchor::value               _ancre_{globals::anchor::fixed};
    globals::uistyle::Type               _uistyle_{globals::uistyle::Unset};
    // --------------------------------------------

    virtual book::code auto_fit(globals::anchor::value anchor_value=globals::anchor::fixed);
    virtual book::code resize(ui::size new_sz);

    //terminal::vchar* _bkcrs_{nullptr}; ///< pointer to vchar at the current rectangle::cursor position.
    terminal::vchar::string::iterator _iterator_{}; ///< Normal std::vector iterator available for navigation and io.
                                                    ///< It is safe to assign an iterator since read-write never invalidate the iterator.


public:
    widget_base();
    widget_base(object* _parent_obj, const std::string _id, globals::uistyle::Type _ui_style=globals::uistyle::Unset);
    ~widget_base() override;


    book::code set_geometry(const rectangle& r);
    book::code set_theme(const std::string& theme_id);
    std::string theme_name(){ return _theme_id_; };

    book::code peek_xy(ui::cxy xy);
    terminal::vchar::string::iterator operator*();


    class _TUXVISION_ painter_dc
    {
        rectangle _geometry_{};
        widget_base* _parent_dc_{nullptr};
        color::pair _colors_{};
        //...
        painter_dc() = default;
        ~painter_dc() = default;
        painter_dc(widget_base* parent_widget, const rectangle& r);

        painter_dc& clear(const rectangle& r);
        painter_dc& operator << (ui::cxy new_xy);
        painter_dc& operator << (char ch);
        painter_dc& operator << (const char* str);
        painter_dc& operator << (const std::string& str);
        painter_dc& operator << (const tux::string& str);
        painter_dc& operator << (color::code fgcol);
        painter_dc& operator << (color::pair colp);
        painter_dc& operator << (cadre::index ci);
        painter_dc& operator << (glyph::type ic);
        painter_dc& operator << (accent_fr::type fr);
        painter_dc& operator << (terminal::vchar vch);
        painter_dc& operator << (terminal::vchar::string strvch);

        painter_dc& set_background_color(color::code bgcol);

        painter_dc& draw_frame(const rectangle& r={});




    };

    book::code draw();
    book::code dirty(const rectangle& dirty_rect={});
    widget_base::painter_dc begin_draw();
    void end_draw(widget_base::painter_dc& edc);

private:

};




} // namespace tux::ui

#endif // WIDGET_BASE_H
