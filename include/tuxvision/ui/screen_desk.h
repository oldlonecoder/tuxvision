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

#pragma once


#include <tuxvision/ui/widget_base.h>
#include <list> // Implementing toplevel floating z-oredered widgets management


namespace tux::ui
{


class _TUXVISION_ screen : public widget_base
{
    CLASSNAME(screen)

    static screen* _screen_;
    static terminal::vchar::back_buffer __back_buffer_; ///< Prepared screen content buffer.
public:
    screen(){}
    screen(const std::string& scr_name);

    ~screen();
    // ...
    static book::code start();
    static book::code end();
    static screen* me();

    book::code draw() override;

protected:
    //friend class widget_base;
    book::code render() override;
    book::code resize(ui::size new_sz) override;
    // --- Toplevel widget management. ( ANY widget can be a toplevel widget )
    book::code show_toplevel(widget_base* wb);
    book::code hide_toplevel(widget_base* wb); //
    book::code toplevel_moved(widget_base* wb);
    book::code pop_widget(widget_base* wb);
    book::code put_front(widget_base* wb); // FAIT,  À TESTER
    book::code push_back(widget_base* wb);
    book::code push_front(widget_base* wb);
    rectangle get_exposed(widget_base* wb);

    //...
    //--------------------------------------------------------------------------
private:
    std::list<widget_base*> _toplevels_{}; ///< toplevels storage in natural z-order
    auto query(widget_base *wb) -> std::list<widget_base*>::iterator;
    void commit_screen();
    book::code render_widget(widget_base* wb);
    book::code dirty_toplevel(widget_base* _toplvl);
    book::code commit(const rectangle& bb_subarea);
    book::code expose(const rectangle& bb_subarea);
    book::code refresh_back_buffer(const rectangle& _area);



};



}
