

#include <tuxvision/ui/widget/label.h>



namespace tux::ui 
{

label::label(object* _parent_widget, const std::string _id): widget_base(_parent_widget,_id){}

book::code label::draw()
{
    clear();
    auto p = begin_draw();
    p << _text_;
    p.home();
    end_draw(p);

    return book::code::done;
}



book::code label::set_text(const std::string& _txt)
{
    _text_ = _txt;
    return draw();
}

}