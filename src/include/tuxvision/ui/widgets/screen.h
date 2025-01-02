//#ifndef TUX_STRING_H
//#define TUX_STRING_H
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
#include <tuxvision/tui/widgets/window.h>



namespace tux::ui
{
class application;

}


namespace tux::ui::terminal
{


/*!
 * @brief screen's double back-buffer to be studied...
 */
class OOM_API screen : public widget
{
    CLASSNAME(screen)

    terminal::vchar::back_buffer _screen_buffer_{nullptr};

    std::vector<rectangle> _dirty_stack_{};
    std::list<window*> _windows_{};

    static screen* the_screen;
    void refresh_zorder();
    void render_screen(const ui::rectangle& rect);

    terminal::vchar::string::iterator peek_sb(ui::cxy xy);
public:
    screen();
    explicit screen(const std::string& _id);
    ~screen() override;

    screen& operator *() override{return *this;}
    rem::code update() override;
    rem::code draw() override;
    rem::code show() override;
    rem::code setup_ui() override;
    ui::status_bar* statusbar();

    rem::code add_window(window* w);
    rem::code show_window(window* w);
    rem::code hide_window(window* w);
    rem::code remove_window(window* w);
    rem::code render_window(window* w, const rectangle& r={});


protected:
    friend class tux::ui::window;
    friend class tux::ui::widget;
    friend class tux::ui::application;

    rem::code dirty(const rectangle& _dirty_rect) override;

private:
    rem::code blit_window(window* _window);
    rem::code blit_buffer(const rectangle& rect);
    rem::code setup();

    status_bar* _status_bar_{nullptr};

};

}
