

#include "tuxvision/ui/application.h"
#include "tuxvision/ui/terminal.h"
#include <csignal>


//#include <memory>


namespace tux::ui
{

application* application::_app_{nullptr};



application::application(const std::string& a_id, int argc, char** argv): _app_name_(a_id)
{
    if(argc && argv)
        tux::string::string_view_list(argc,argv);

    if(application::_app_ == nullptr)
    {
        application::_app_ = this;
    }
    else
        abort();
}


application::~application()
{
    //...
    _args_.clear();
    ///...
}


void application::sig_interrupted(int)
{
    book::interrupted();
    //terminal::end();
    //book::purge(nullptr);
    application::app().terminate();
    exit(0);
}



void application::sig_aborted(int)
{
    book::aborted();
    //terminal::end();
    //book::purge(nullptr);
    application::app().terminate();
    exit(0);
}

void application::sig_crash(int)
{
    book::segfault();
    //terminal::end();
    //book::purge(nullptr);
    application::app().terminate();
    exit(0);
}


void application::sig_winch(int)
{
    terminal::query_winch();
    //...
}



application& application::app()
{
    if(!application::_app_)
        throw book::exception() [book::except() << book::fn::fun << book::fn::endl << " no instance of application!"];

    return *application::_app_;
}

// desktop *application::screen_desktop()
// {
//     return desktop::instance();
// }

book::code application::install_signals()
{

    std::signal(SIGSEGV, &application::sig_crash);
    book::log() << "signal SIGSEV installed.";
    std::signal(SIGABRT, &application::sig_aborted);
    book::log() << "signal SIGABRT installed.";
    std::signal(SIGINT, &application::sig_interrupted);
    book::log() << "signal SIGINT installed.";
    std::signal(SIGWINCH, &application::sig_winch);
    book::log() << "signal SIGWINCH installed.";
    // std::signal(SIGHUP, &application::sig_winch);
    std::signal(SIGKILL, &application::sig_interrupted);
    book::log() << "signal SIGKILL installed.";
    std::signal(SIGTERM, &application::sig_interrupted);
    book::log() << "signal SIGTERM installed.";

    return book::code::done;
}

book::code application::setup()
{
    //...
    book::init();
    install_signals();
    terminal::begin();
    return book::code::done;
}


book::code application::terminate()
{
    terminal::end();
    return book::code::reimplement;
}




size_t application::push_event(event&& ev)
{
    _events_stack_.push(ev);
    return _events_stack_.size();
}


event  application::pop_event()
{
    if(_events_stack_.empty())
    {
        book::error() << book::fn::fun << book::code::empty;
        return {};
    }

    auto ev = _events_stack_.top();
    _events_stack_.pop();
    return ev;
}


}
