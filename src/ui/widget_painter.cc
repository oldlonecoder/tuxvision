#include <tuxvision/ui/widget_base.h>






namespace tux::ui
{

/*!
 * \brief Instance public constructor widget_base::painter_dc::painter_dc
 * \param parent_widget
 * \param r
 */
widget_base::painter_dc::painter_dc(widget_base *parent_widget, const rectangle &r):_parent_dc_(parent_widget), _geometry_(r)
{
    _colors_ = _parent_dc_->_colors_;
    if(!r)
        _geometry_ = _parent_dc_->_geometry_.tolocal();

    //...
}


/*!
 * \brief Instance public widget_base::painter_dc::clear
    clears the (sub-)area \c r with the current colors attributes.
 * \param r
 * \return ref to self.
 * \note r must be local coordinates (offset from {0,0}).
 */
widget_base::painter_dc &widget_base::painter_dc::clear(const rectangle &r)
{
    auto area = r;
    if(!area)
    {
        _parent_dc_->clear(); // no need the line-by-line overhead here;
        return *this;
    }

    if(area = _geometry_.tolocal() & r; !area)
    {
        book::error() << book::fn::fun << "'" << color::yellow << _parent_dc_->id()
                      << color::reset << "': request clear "
                      << color::lightsteelblue3 << r
                      << color::reset << " is out of boundaries." ;

        return *this;
    }

    for(int y = 0; y < area.dwh.h; y++)
    {
        auto it = _parent_dc_->position(area.a + ui::cxy{0,y});
        std::fill(it, it + area.dwh.w, terminal::vchar(_colors_));
    }

    return *this;
}

/*!
 * \brief Instance public widget_base::painter_dc::set_background_color
 * \param bgcol ...
 * \return ref to self.
 */
widget_base::painter_dc &widget_base::painter_dc::set_background_color(color::code bgcol)
{
    _colors_.bg = bgcol;
    return *this;
}

/*!
 * \brief Instance public widget_base::painter_dc::draw_frame
 * \param r ...
 * \return ref to self
 */
widget_base::painter_dc &widget_base::painter_dc::draw_frame(const rectangle &r)
{
    auto area = r;
    if(!area)
        area = _parent_dc_->_geometry_.tolocal();

    area = _geometry_.tolocal() & r; // Hence {0,0}.
    if(!area)
    {
        book::error() << book::fn::fun << "'" << color::yellow << _parent_dc_->id()
        << color::reset << "': requested "
        << color::lightsteelblue3 << r
        << color::reset << " is out of boundaries." ;

        return *this;
    }


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

