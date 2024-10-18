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



#include <tuxvision/ui/screen_desk.h>

namespace tux::ui
{


#define UPDATE_ZORDER \
for(auto it = _toplevels_.begin(); it != _toplevels_.end(); it++) (*it)->_tli_ = it;



screen* screen::_screen_{nullptr};
terminal::vchar::back_buffer screen::__back_buffer_; ///< Prepared screen content buffer.


/*!
 * \brief Static public screen::me
 * \return  singleton instance of the app screen.
 *
 */
screen *screen::me()
{
    // ... Handle idiot calls ...
    return screen::_screen_;
}



/*!
 * \brief Instance public screen::render_widget
 *     Renders the dirty_area of the given top-level widget's back buffer, into this screen's back buffer.
 * \param wb
 * \return rejected or accpeted;
 */
book::code screen::render_widget(widget_base *wb)
{
    rectangle area = wb->_dirty_area_;
    if(!area)
        area = me()->_geometry_ & wb->_geometry_;
    if(!area)
    {
        book::debug() << wb->id() << ": is not 'visible' on the screen's geometry - rendering rejected.";
        return book::code::rejected;
    }

    for(int y = 0; y < area.dwh.h; y++)
    {
        me()->peek_xy(area.a + ui::cxy{0,y});
        wb->peek_xy((area.a + ui::cxy{0,y}) - wb->_geometry_.a);
        (void)std::copy(wb->_iterator_, wb->_iterator_ + area.dwh.w, me()->_iterator_);
        // ignore result iterator (i.e.: the returned value of me()->_iterator_).
        // Ignorer la nouvelle valeur de l'iterateur retourn&eacute;
        // -------------------
        //std::memcpy(&(me()->_iterator_),wb->vc(), area.dwh.w*sizeof(terminal::vchar));
        //--------------------
    }
    me()->dirty(area); // or let's evaluate something like ' push_dirty(area) instead of growing the screen's dirty rectangle... ;)
    return book::code::accepted;
}




/*!
 * \brief public instance constructor
 * \param scr_name
 */
screen::screen(const std::string &scr_name): widget_base(nullptr, scr_name)
{
    if(screen::_screen_)
        return;

    screen::_screen_ = this;
    //...
}

screen::~screen()
{

}

/*!
 * \brief static public screen::start
 * \return ready/rejected/failed/exist
 */
book::code screen::start()
{
    if(me())
        return book::code::exist;

    new screen("screen ios");
    terminal::begin();
    screen::_screen_->set_geometry(terminal::geometry());
    screen::__back_buffer_ = std::make_shared<terminal::vchar::string>(screen::_screen_->_geometry_.dwh.area(), terminal::vchar(screen::_screen_->_colors_));

    book::out() << color::blueviolet <<  screen::_screen_->class_name() << color::grey100 << "::" << color::yellow << screen::_screen_->id() << color::reset << " double back_buffer setup complete.";
    //...
    return book::code::ready;
}



/*!
 * \brief static public screen::end
 * \return book::code::terminate
 */
book::code screen::end()
{
    //...
    terminal::end();
    delete screen::_screen_;
    return book::code::terminate;
}

//-------------------------------------------------------------------

// --- Toplevel widget management. ( ANY widget can be a toplevel widget )


book::code screen::resize(ui::size new_sz)
{
    return book::code::notimplemented;
}

book::code screen::show_toplevel(widget_base* wb)
{
    return book::code::notimplemented;
}

// book::code screen::draw_toplevel(widget_base* wb)
// {

//     return book::code::notimplemented;
// }

book::code screen::hide_toplevel(widget_base* wb)
{
    //pop_widget(wb);
    dirty_toplevel(wb);

    return book::code::notimplemented;
}

book::code screen::toplevel_moved(widget_base* wb)
{
    return book::code::notimplemented;
}

book::code screen::pop_widget(widget_base* wb)
{
    if(auto it = query(wb); it != _toplevels_.end())
    {
        _toplevels_.erase(it);
        UPDATE_ZORDER
        return book::code::complete;
    }

    return book::code::notexist;
}


book::code screen::put_front(widget_base* wb)
{
    UPDATE_ZORDER

    auto it = _toplevels_.end();

    if(it = query(wb); it == _toplevels_.end())
        return book::code::notexist;

    if(wb == _toplevels_.back())
        return book::code::implemented;

    _toplevels_.erase(it);
    UPDATE_ZORDER
    _toplevels_.push_back(wb);
    UPDATE_ZORDER

    return book::code::complete;
}

book::code screen::push_back(widget_base *wb)
{
    return book::code::notimplemented;
}

/*!
 * \brief screen::push_front un nouvel arrivant chez les toplevels!
 * \param wb
 * \return  accepted if added, rejected if exists or not a toplevel widget(_base)
 */
book::code screen::push_front(widget_base *wb)
{
    // Refresh
    UPDATE_ZORDER

    auto it = _toplevels_.end();

    if(it = query(wb); it != _toplevels_.end())
        return book::code::exist;

    if(!wb->is_toplevel())
    {
        book::error() << book::fn::fun << book::code::cancel << " widget '" << color::yellow << wb->class_name() << "::" << wb->id() << color::reset << "' is not a top-level widget. Operation Cancelled";
        return book::code::cancel;
    }

    _toplevels_.push_back(wb);
    // --- todo: 'bitblit' wb onto __back_buffer.
    _dirty_area_ = wb->_geometry_;
    auto local = wb->_geometry_.tolocal();
    _dirty_area_ = _dirty_area_ & _geometry_.tolocal();
    if(!_dirty_area_)
    {
        book::message() << book::code::oob << wb->id() << "' is not visible, reject screen update";
        return book::code::rejected;
    }
    local += ui::cxy{_dirty_area_.a};
    local = _dirty_area_ & local;
    local -= _dirty_area_.a;

    for(int y = 0; y < local.dwh.h; y++)
    {
        wb->peek_xy(local.a+ui::cxy{0,y});
        auto bbit = screen::__back_buffer_->begin() + _dirty_area_.a.x + _dirty_area_.a.y * _geometry_.dwh.w;
        std::copy(wb->_iterator_, wb->_iterator_ + *local.width<size_t>(), bbit);
    }
    wb->_tli_ = --_toplevels_.end(); //expose(wb->_geometry_);

    UPDATE_ZORDER
    return book::code::accepted;
}



/*!
 * \brief Instance protected (internal) virtual screen::dirty
 * \param _toplvl Top level widget to refresh on the back buffer.
 * \return  accepted or rejected if _toplvl is not visible within screen's geometry.
 */
book::code screen::dirty_toplevel(widget_base *_toplvl)
{
    // take the dirty area of the toplvl widget to expose :
    UPDATE_ZORDER;

    if(query(_toplvl) == _toplevels_.end())
    {
        return book::code::rejected;
    }

    //auto area = _toplvl->_dirty_area_ + _toplvl->_geometry_.a; // to screen scale.

    // Un mixte de vieux et de nouveau code hihihihihih:
    // Mixing old and new code:
    rectangle area{};
    if(area = _toplvl->_dirty_area_ + _toplvl->_geometry_.a; !(screen::me()->_geometry_.tolocal() & area))
    {
        book::status() << book::fn::fun << book::code::oob << " :" << _toplvl->_dirty_area_ << " <> " << screen::me()->_geometry_.tolocal();
        return book::code::rejected;
    }
    std::next(_toplvl->_tli_);

    expose(area);

    return book::code::notimplemented;
}

book::code screen::expose(const rectangle &bb_subarea)
{
    book::debug() << book::fn::fun << color::yellow << id() << color::reset << "::expose() : width:" << (_iterator_ + *_geometry_.width())-_bloc_->begin();
    //_dirty_area_ = {};
    for(int y=0; y < _dirty_area_.dwh.h; y++)
    {
        auto bbit = screen::__back_buffer_->begin() + _dirty_area_.a.x + _dirty_area_.a.y * _geometry_.dwh.w;
        terminal::cursor({_dirty_area_.a + ui::cxy{0,y}});
        terminal::vchar::render_string(bbit, bbit + _dirty_area_.dwh.w);
    }
    std::cout  << std::flush;
    _dirty_area_ = {};
    return book::code::complete;
}


auto screen::query(widget_base *wb) -> std::list<widget_base*>::iterator
{
    for(auto it = _toplevels_.begin(); it != _toplevels_.end(); it++)
        if(*it == wb) return it;

    return _toplevels_.end();
}

void screen::commit_screen()
{
    auto r = _dirty_area_;
    if(!r)
        r = _geometry_;

    render();
}

// -----------------------------------------------------


// /*!
//  * \brief Instance public screen::dirty
//  * \param _r  validate boundaries before applying dirty_area union with _r.
//  * \return rejected if _r is invalid; accepted and applied if valid and within _geometry_.
//  */
// book::code screen::dirty(const rectangle &_r)
// {
//     auto r = _geometry_ & _r;
//     if(!r) return book::code::rejected;
//     _dirty_area_ |= r;


//     return book::code::accepted;
// }


book::code screen::render()
{
    return widget_base::render();
}

book::code screen::draw()
{
    auto writer = begin_draw();
    writer.clear();
    end_draw(writer);

    return book::code::notimplemented;
}


}


