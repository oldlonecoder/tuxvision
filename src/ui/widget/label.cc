

#include <tuxvision/ui/widget/label.h>



namespace tux::ui
{

label::label(object* _parent_widget, const std::string _id): widget_base(_parent_widget,_id){}

label::~label()
{
    _text_.clear();
}

book::code label::draw()
{

    book::debug() << book::fn::fun;
    book::out() << "[" << id() << "]" << "geometry: " << color::lime << _geometry_ << color::reset << book::fn::endl;
    clear();
    auto p = begin_draw();
    p.home();
    p << _text_;
    end_draw(p);

    return book::code::done;
}



book::code label::set_text(const std::string& _txt)
{
    book::log() << book::fn::fun;
    _text_ = _txt;
    book::out() << " text set to '" << _text_ << "';";
    return draw();
}

}
