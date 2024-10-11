#ifndef WIDGET_BASE_H
#define WIDGET_BASE_H

#include <tuxvision/tools/object.h>
#include <tuxvision/ui/globals.h>
namespace tux::ui
{


class _TUXVISION_ widget_base : public object
{
public:
    widget_base();
    widget_base(object* _parent_obj, const std::string _id, globals::uistyle::Type _ui_style=globals::uistyle::Unset);
    ~widget_base() override;

};

} // namespace ui
} // namespace tux

#endif // WIDGET_BASE_H
