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

book::code widget_base::painter_dc::operator ++()
{
    if(_parent_dc_->_iterator_ == _parent_dc_->_bloc_->end())
        return book::code::rejected;
    ++_parent_dc_->_iterator_;
    return book::code::accepted;
}

book::code widget_base::painter_dc::operator +=(size_t _offset)
{
    if(_parent_dc_->_iterator_ >= _parent_dc_->_bloc_->end())
    {
        _parent_dc_->_iterator_ = -- _parent_dc_->_bloc_->end();
        return book::code::rejected;
    }

    _parent_dc_->_iterator_ += _offset;
    return book::code::accepted;
}


widget_base::painter_dc& widget_base::painter_dc::operator << (ui::cxy new_xy)
{
    if(!_parent_dc_->peek_xy(new_xy + _geometry_.a))
        throw book::exception()
        [
            book::except() << book::fn::fun << book::code::oob << color::red4 << new_xy << color::reset << " > " << color::yellow << _parent_dc_->_geometry_.tolocal()
        ];

    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (char ch)
{
    (*_parent_dc_->_iterator_) << ch;
    ++**this;
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const char* str)
{
    auto z = std::strlen(str);
    auto start = _parent_dc_->_iterator_;
    auto it = start + z;

    if(it >= _parent_dc_->_bloc_->end())
        it = _parent_dc_->_bloc_->begin() + (it - (_parent_dc_->_bloc_->end()+1));

    auto c = str;
    for(; start <=it; start++)
        (*start) << *c++;
    **this += it-_parent_dc_->_bloc_->begin();
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const std::string& str)
{
    return **this << str.c_str();
}

widget_base::painter_dc& widget_base::painter_dc::operator << (const tux::string& str)
{
    //return **this << str.c_str();
}

widget_base::painter_dc& widget_base::painter_dc::operator << (color::code fgcol)
{
    _colors_.fg = fgcol;
    _parent_dc_->_iterator_->set_fg(fgcol);

    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (color::pair colp)
{
    _colors_ = colp;
    _parent_dc_->_iterator_->set_colors(colp);
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (cadre::index ci)
{
    (*_parent_dc_->_iterator_) << ci;
    ++**this;
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (glyph::type ic)
{
    (*_parent_dc_->_iterator_) << ic;
    ++**this;
    return *this;
}

widget_base::painter_dc& widget_base::painter_dc::operator << (accent_fr::type fr)
{
    (*_parent_dc_->_iterator_) << fr;
    ++**this;
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

