//#include "tuxvision/journal/book.h"

//-------------------------------------
//#include "tux/tools/sscan.h"
//#include <cstdint>
#include <tuxvision/est/expression.h>
#include <tuxvision/ui/events.h>
#include <tuxvision/ui/widget_base.h>
#include <tuxvision/ui/application.h>
//=====================================

#include <tuxvision/journal/book.h>
#include <unistd.h>


namespace tux::ui
{
class app : public application
{

    // intrack*        intrack_ptr{nullptr};
    // //ui::statusbar*  statusbar{nullptr};
    // desktop*         desk{nullptr};
    // window*         C64Win{nullptr};
public:

    app(const std::string& app_name, int argc, char **argv);
    ~app() override;
    book::code run() override;
    book::code terminate() override;
protected:
    book::code setup() override;
    book::action terminal_resize_signal(rectangle _r);
    book::code setup_ui() override;
    book::code expression();
};



book::code app::setup()
{
    application::setup();
    terminal::term_resize_signal().connect(this, &app::terminal_resize_signal);
    //...
    return book::code::done;
}


book::action app::terminal_resize_signal(rectangle _r)
{
    return book::action::leave;
}



app::app(const std::string &app_name, int argc, char **argv):application(app_name, argc,argv){}

app::~app(){}

book::code app::run()
{

    setup();
    auto r = expression();
    book::status() << book::fn::fun << "app:expression() test: " << r;
    auto widget = widget_base{nullptr,"test with shared mem"};
    widget.set_theme("C128");
    widget.set_geometry(terminal::geometry());
    //widget.dirty({});
    terminal::cursor({1,1});


    event ev;
    book::code c{};
    while(!!(c = event::get_stdin_event(ev,{65000,0})) && c != book::code::cancel);
    terminate();
    return book::code::terminate;
}

book::code app::terminate()
{
    application::terminate();
    book::purge(nullptr);
    return book::code::terminate;
}


book::code app::setup_ui()
{
    return book::code::notimplemented;
}




book::code app::expression()
{

    tux::est::expr e("est - Expression Syntax Tree");
    e = "a = 0.02 / ( 0.02 - 42);";
    //tux::est::alu aa =e.exec("a = 0.02 / ( 0.02 - 42);");
    tux::est::alu val =e.exec( );
    book::out() << tux::color::reset << '\''
                << tux::color::dodgerblue3 << e.id()
                << tux::color::reset << "': est::alu "
                << tux::color::yellow << "val="
                << tux::color::lime <<  val() << book::fn::endl;
    if(auto const& variables = e.variables(); !variables.empty())
    {
        book::status() << book::fn::fun << color::yellow << e.id() << color::reset << " Result (var dump):";
        for(auto var : variables)
            book::out() << color::yellow << var->id() << color::reset << " := " << color::lime << var->value().value<float>() << color::reset;

    }
    else
        book::message() << book::fn::fun << " expression has no (local) variables";
    book::log() << book::fn::fun << " Complete.";
    return book::code::ok;
}

}


auto main(int argc, char** argv)->int
{
    tux::string str;
    str | color::blue4 | "hello" | color::grey100 | ", and welcome to the " | color::blueviolet | "tuxvision " | color::reset | " project!!";
    std::cout << str() << std::endl;

    tux::ui::app app{"tuxvision tests", argc, argv};
    app.run();
    return 0;
}

