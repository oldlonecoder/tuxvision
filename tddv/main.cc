//#include "tuxvision/journal/book.h"

//-------------------------------------
//#include "tuxin/tools/sscan.h"
#include "tuxvision/est/expression.h"
//#include "tuxin/tui/events.h"
//#include "tuxin/tools/application.h"
//=====================================

#include <tuxvision/journal/book.h>

#include <unistd.h>

book::code expression()
{

    tux::est::expr e("est - Expression Syntax Tree");
    e = "a = 0.02 / ( 0.02 - 42);";
    //tux::est::alu aa =e.exec("a = 0.02 / ( 0.02 - 42);");
    tux::est::alu val =e.exec( );
    book::out() << tux::color::reset << '\''
                << tux::color::dodgerblue3 << e.id()
                << tux::color::reset << "': est::alu "
                << tux::color::yellow << "val="
                << tux::color::lime <<  val()
                << tux::color::reset;

    return book::code::ok;
}



auto main(int argc, char** argv)->int
{
    book::init();
    tux::string str;
    str | color::blue4 | "hello" | color::grey100 | ", and welcome to the " | color::blueviolet | "tuxvision " | color::reset | " project!!";
    std::cout << str() << std::endl;

    expression();

    book::debug() << book::fn::fun << color::blue4 << "hello" << color::grey100 << ", and welcome to the " << color::blueviolet << "tuxvision " << color::reset << " project!!";

    book::purge(nullptr);
    return 0;
}
