#include <tuxvision/ui/widget_base.h>






namespace tux::ui
{


widget_base::painter_dc::painter_dc(widget_base *parent_widget, const rectangle &r):_parent_dc_(parent_widget), _geometry_(r)
{
    _colors_ = _parent_dc_->_colors_;
    //...
}

widget_base::painter_dc &widget_base::painter_dc::clear(const rectangle &r)
{
    return *this;
}

widget_base::painter_dc &widget_base::painter_dc::set_background_color(color::code bgcol)
{
    return *this;
}

widget_base::painter_dc &widget_base::painter_dc::draw_frame(const rectangle &r)
{
    return *this;
}


widget_base::painter_dc& widget_base::painter_dc::operator << (ui::cxy new_xy)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (char ch)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const char* str)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const std::string& str)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const tux::string& str)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (color::code fgcol)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (color::pair colp)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (cadre::index ci)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (glyph::type ic)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (accent_fr::type fr)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (terminal::vchar vch)
{
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (terminal::vchar::string strvch)
{
    return *this;
}



} // namespace tux::ui

