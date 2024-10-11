//#include "tuxvision/journal/book.h"

//-------------------------------------
//#include "tuxin/tools/sscan.h"
//#include "tuxin/est/expression.h"
//#include "tuxin/tui/events.h"
//#include "tuxin/tools/application.h"
//=====================================

#include <tuxvision/journal/book.h>

#include <unistd.h>



auto main(int argc, char** argv)->int
{
    tux::string str;
    str | color::blue4 | "hello" | color::grey100 | ", and welcome to the " | color::blueviolet | "tuxvision " | color::reset | " project!!";
    std::cout << str() << std::endl;
    
    return 0;
}
