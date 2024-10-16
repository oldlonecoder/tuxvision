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

screen *screen::me()
{
    return screen::_screen_;
}

book::code screen::render_widget(widget_base *wb)
{

    return book::code::notimplemented;
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
    book::init();
    terminal::begin();
    screen::_screen_->set_geometry(terminal::geometry());
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

// -----------------------------------------------------



book::code screen::dirty(const rectangle &_r)
{
    return book::code::notimplemented;
}

book::code screen::render()
{
    return book::code::notimplemented;
}

book::code screen::draw()
{
    return book::code::notimplemented;
}


}


