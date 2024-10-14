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
    rectangle _geometry_{};
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
public:
    widget_base();
    widget_base(object* _parent_obj, const std::string _id, globals::uistyle::Type _ui_style=globals::uistyle::Unset);
    ~widget_base() override;


    book::code set_geometry(const rectangle& r);
    book::code set_theme(const std::string& theme_id);
    std::string theme_name(){ return _theme_id_; };



    class _TUXVISION_ painter_dc
    {
        rectangle _geometry_{};
        widget_base* _parent_dc_{nullptr};
        color::pair _colors_{};
        //...
    };

    book::code draw();
    book::code dirty(const rectangle& dirty_rect={});
    widget_base::painter_dc begin_draw();
    void end_draw(widget_base::painter_dc& edc);

private:

};




} // namespace tux::ui

#endif // WIDGET_BASE_H
