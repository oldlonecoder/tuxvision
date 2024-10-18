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
for(auto it = _toplevels_.begin(); it != _toplevels_.end(); it++)\
    (*it)->_tli_ = it;


screen* screen::_screen_{nullptr};
terminal::vchar::back_buffer screen::_toplevels_bb_; ///< Prepared screen content buffer.
screen *screen::me()
{
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
    screen::_toplevels_bb_ = std::make_shared<terminal::vchar::string>(screen::_screen_->_geometry_.dwh.area(), terminal::vchar(screen::_screen_->_colors_));

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

book::code screen::draw_toplevel(widget_base* wb)
{

    return book::code::notimplemented;
}

book::code screen::hide_toplevel(widget_base* wb)
{
    pop_widget(wb);

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
 * \return
 */
book::code screen::push_front(widget_base *wb)
{
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
    UPDATE_ZORDER

    return book::code::accepted;
}

rectangle screen::get_exposed(widget_base* wb)
{
    return {};
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


}

// -----------------------------------------------------


/*!
 * \brief Instance public screen::dirty
 * \param _r  validate boundaries before applying dirty_area union with _r.
 * \return rejected if _r is invalid; accepted and applied if valid and within _geometry_.
 */
book::code screen::dirty(const rectangle &_r)
{
    auto r = _geometry_ & _r;
    if(!r) return book::code::rejected;
    _dirty_area_ |= r;

    return book::code::accepted;
}


book::code screen::render()
{
    return book::code::notimplemented;
}

book::code screen::draw()
{
    auto writer = begin_draw();
    writer.clear();

    return book::code::notimplemented;
}


}


