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


/*!
 *  @defgroup dirty
 *  \brief Refresh dirty area routines.
 */

namespace tux::ui
{


#define UPDATE_ZORDER \
for(auto it = _windows_.begin(); it != _windows_.end(); it++) (*it)->_tli_ = it;



screen* screen::_screen_{nullptr};
terminal::vchar::back_buffer screen::__back_buffer_; ///< Prepared screen content buffer.


/*!
 * @brief Static public screen::me
 * @return  singleton instance of the app screen.
 *
 */
screen *screen::me()
{
    // ... Handle idiot calls ...
    return screen::_screen_;
}






/*!
 * @brief public instance constructor
 * @param scr_name
 */
screen::screen(const std::string &scr_name): widget_base(nullptr, scr_name)
{
    if(screen::_screen_)
        return;

    screen::_screen_ = this;
    _uiflags_ = globals::wflags::TopLevel;
    //...
}

screen::~screen()
{

}

/*!
 * @brief static public screen::start
 * @return ready/rejected/failed/exist
 */
book::code screen::start()
{
    if(me())
        return book::code::exist;

    new screen("screen ios");
    terminal::begin();
    screen::_screen_->set_theme("C128");
    screen::_screen_->set_geometry(terminal::geometry());

    screen::__back_buffer_ = std::make_shared<terminal::vchar::string>(screen::_screen_->_geometry_.dwh.area(), terminal::vchar(screen::_screen_->_colors_));

    book::out() << color::blueviolet <<  screen::_screen_->class_name() << color::grey100 << "::" << color::yellow << screen::_screen_->id() << color::reset << " double back_buffer setup complete.";
    //...
    return book::code::ready;
}



/*!
 * @brief static public screen::end
 * @return book::code::terminate
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

/*!
 * @brief screen::resize
 * @param new_sz
 * @return
 */
book::code screen::resize(ui::size new_sz)
{
    widget_base::resize(new_sz);
    screen::__back_buffer_->resize(new_sz.area(), terminal::vchar(_colors_));
    draw();
    return book::code::done;
}



book::code screen::show_window(widget_base* wb)
{
    return book::code::notimplemented;
}





/*!
 * @brief screen::__iterate_windows
 * For each toplevel widgets, call ffn with the exposing rectangle and the line number to expose within the exposed rectangle.
 * @param r
 * @param y
 * @param ffn
 */
void screen::__iterate_windows(const rectangle &r, int y, std::function<void (widget_base *, const rectangle &, int)> ffn)
{
    for(auto *w: screen::_windows_) ffn(w,r,y);
}

terminal::vchar::iterator  screen::peek_bb(cxy xy)
{
    if(!_geometry_.tolocal()[xy])
        return __back_buffer_->end();

    return __back_buffer_->begin() + xy.x + *_geometry_.width() * xy.y;
}





/*!
 * \brief screen::hide_window
 * \param wb
 * \return
 */
book::code screen::hide_window(widget_base* wb)
{
    _dirty_area_ = _geometry_ & wb->_geometry_;
    if(!_dirty_area_)
        return book::code::rejected;

    _windows_.erase(wb->_tli_);
    UPDATE_ZORDER
    return refresh_back_buffer(_dirty_area_);
}


/*!
 * @brief screen::to_back
 *      Put window to the front of the windows list.
 * @param wb
 * @return accepted or rejected if wb id not in the windows list.
 */
book::code screen::to_back(widget_base* wb)
{
    UPDATE_ZORDER

    auto it = _windows_.end();

    if(it = query(wb); it == _windows_.end())
        return book::code::notexist;

    if(wb == _windows_.back())
        return book::code::done;

    _windows_.erase(it);
    _windows_.push_back(wb);
    UPDATE_ZORDER
    // Setup window's _durty_area_ :
    wb->_dirty_area_ = _geometry_ & wb->_geometry_;
    if(!wb->_dirty_area_)
        return book::code::rejected;

    wb->_dirty_area_ -= wb->_geometry_.a;
    expose_window_to_bb(wb);

    return book::code::done;
}

/*!
 * @brief screen::push_back un nouvel arrivant chez les toplevels!
 * @param wb
 * @return  accepted if added, rejected if exists or not a toplevel widget(_base)
 */
book::code screen::push_back(widget_base *wb)
{
    // Refresh

    if(auto it = query(wb); it != _windows_.end())
        return book::code::exist;

    if(!wb->is_toplevel())
    {
        book::error() << book::fn::fun << book::code::cancel << " widget '" << color::yellow << wb->class_name() << "::" << wb->id() << color::reset << "' is not a top-level widget. Operation Cancelled";
        return book::code::cancel;
    }

    _windows_.push_back(wb);
    // --- todo: 'bitblit' wb onto __back_buffer.
    UPDATE_ZORDER


    wb->_dirty_area_ = _geometry_.tolocal();
    expose_window_to_bb(wb);
    return book::code::done;
}



// /*!
//  * @brief Instance protected (internal) virtual screen::dirty routine
//  * @param _toplvl Top level widget to refresh on the back buffer.
//  * @note _toplvl->_dirty_area_ must be set prior to call this function/method.
//  * @return  accepted or rejected if _toplvl is not visible within screen's geometry.
//  */
// book::code screen::dirty_window(widget_base *_toplvl)
// {
//     // take the dirty area of the toplvl widget to expose :

//     if(query(_toplvl) == _windows_.end())
//         return book::code::rejected;

//     expose_window_to_bb(_toplvl);
//     return book::code::complete;
// }


/*!
 * @brief screen::expose_window_to_bb
 *      Expose onto screen::__back_buffer's sub _area,  the given toplevel widget_base*.
 * @param w toplevel widget
 * @param _area exposed area.
 * @note Assuming that w->_dirty_are_ is valid.
 */
void screen::expose_window_to_bb(widget_base *w)
{

    // to screen offset coords:
    book::log() << book::fn::fun << color::lime << class_name() << ":";
    auto area = w->_dirty_area_ + w->_geometry_.a;
    if(w == this)
    {
        refresh_back_buffer(area);
        return;
    }

    for(int y = 0; y < area.dwh.h; y++)
    {
        w->peek_xy({w->_dirty_area_.a.x,w->_dirty_area_.a.y+y});
        std::copy(w->_iterator_, w->_iterator_ + area.dwh.w, peek_bb(area.a + ui::cxy{0,y} ));
    }
}

/*!
 * @brief Private[internal] instance screen::refresh_back_buffer
 *
 *  * Refresh area of screen widget back buffer onto the "desktop" back buffer.
 * Iterates top-level widgets, z-ordered, in intersection with the affected area \c _area
 *
 * @param ui::rectnalge _area : affected area.
 * @return complete.
 */
book::code screen::refresh_back_buffer(const rectangle& _area )
{
    book::log() << book::fn::fun << color::lime << class_name() << ": area to put onto the screen bb:"  << color::yellow << _area;
    book::out() << "test the first line of the widget's back_buffer:" << book::fn::endl;
    peek_xy(_area.a+ui::cxy{0,0});
    book::out() << terminal::vchar::render(&(*_iterator_), _geometry_.dwh.w);

    for(int y = 0; y < _area.height(); y++)
    {
        peek_xy(_area.a+ui::cxy{0,y});// {0,0}!!! NOT _area.a !!!!
        //book::log() << "debug: " << book::fn::fun << "desktop: line #" << color::red4 << y << color::reset << ':';
        //book::out() << _iterator_->details();

        std::copy(_iterator_, _iterator_ + *_area.width(),  peek_bb(_area.a + ui::cxy{0,y}));
    }

    for(auto* tlw : screen::_windows_)
    {
        tlw->_dirty_area_ = tlw->_geometry_ & _area;
        if(!tlw->_dirty_area_) continue;
        tlw->_dirty_area_ -= tlw->_geometry_.a;
        expose_window_to_bb(tlw);
    }
    // testing : push now to the terminal console screen :
    commit(_area);
    return book::code::done;
}


/*!
 * @brief Private instance screen::commit
 *      Commits given area of the back buffer array to the screen (console terminal)
 * @param bb_subarea ( must be pre-validatd ).
 * @return book::code::complete
 */
book::code screen::commit(const rectangle &bb_subarea)
{
    book::debug() << book::fn::fun << color::yellow << id() << color::reset << "::commit() area:" << color::lightsteelblue3 << bb_subarea;

    ui::cxy scr{};
    for(int y=0; y < bb_subarea.dwh.h; y++)
    {
        scr = bb_subarea.a + ui::cxy{0,y} ;
        auto bbit = peek_bb(scr);
        //book::out() << "line#" << color::yellow6 << y << color::grey100 << ":" << bbit->details();
        terminal::cursor(scr+ui::cxy{1,1}); // {1,1}
        terminal::vchar::render_string(bbit, bbit + bb_subarea.dwh.w);
    }
    std::cout  << std::flush;
    return book::code::complete;
}




auto screen::query(widget_base *wb) -> std::list<widget_base*>::iterator
{
    for(auto it = _windows_.begin(); it != _windows_.end(); it++)
        if(*it == wb) return it;

    return _windows_.end();
}

void screen::commit_screen()
{
    auto r = _dirty_area_;
    if(!r)
        r = _geometry_;
}

// -----------------------------------------------------


// /*!
//  * @brief Instance public screen::dirty
//  * @param _r  validate boundaries before applying dirty_area union with _r.
//  * @return rejected if _r is invalid; accepted and applied if valid and within _geometry_.
//  */
// book::code screen::dirty(const rectangle &_r)
// {
//     auto r = _geometry_ & _r;
//     if(!r) return book::code::rejected;
//     _dirty_area_ |= r;


//     return book::code::accepted;
// }



book::code screen::draw()
{
    return widget_base::draw();
}



/*!
 * \brief screen::update
 * For now there is no paticular role. Just calling the base class update. until more specific code is needed.
 * \return widget_base::update() as of oct 2024...
 */
book::code screen::update()
{
    widget_base::update();
    return refresh_back_buffer(_dirty_area_);
}


}


