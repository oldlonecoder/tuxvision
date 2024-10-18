#include <tuxvision/ui/widget_base.h>

namespace tux::ui
{


#define CHECK_BLOC \
if(!_bloc_)\
{\
    throw book::exception() [\
        book::except() << book::fn::fun << book::code::null_ptr << " undefined backbuffer on: "\
                       << color::lightsteelblue << class_name()\
                       << color::reset <<"::"\
                       << color::yellow << id()\
    ];\
}



using namespace globals;


widget_base::widget_base()
{

}

widget_base::widget_base(object *_parent_obj, const std::string _id, globals::uistyle::Type _ui_style):object(_parent_obj, _id),
    _uistyle_(_ui_style){}

widget_base::~widget_base(){}

book::code widget_base::set_geometry(const rectangle &r)
{
    book::info() << color::yellow << id() << color::reset << " requested geometry:" << r;
    if(!r)
    {
        book::error() << book::fn::fun << book::code::null_ptr << " - " << color::yellow << id() << " invalid rectangle!";
        return book::code::rejected;
    }
    _geometry_ = r;
    if(auto p = parent<widget_base>(); p)
    {
        book::out() << "parent(" << color::yellow << p->id() << color::reset << ") bloc assigned to " << id() << ".";
        if(_uiflags_ & globals::wflags::TopLevel)  goto TOPLVL;
        _bloc_ = p->_bloc_;
    }
    else
    {
TOPLVL:
        _bloc_ = std::make_shared<terminal::vchar::string>(_geometry_.dwh.area(), terminal::vchar(_colors_));
        _uiflags_ |= globals::wflags::TopLevel|globals::wflags::Floating;
        book::out() << color::lightsteelblue <<  class_name() << color::grey100 << "::" << color::yellow << id() << color::reset << " is toplevel widget, owns back_buffer";
        book::out() << color::yellow << id() << color::reset << " assisgned geometry:" << _geometry_;
    }
    //_bkcrs_ =
    _iterator_ = _bloc_.get()->begin();
    auto& cell = _bloc_.get()[0][0];
    cell << 'A';

    return book::code::done;
}

/*!
 * \brief widget_base::set_theme
 * \param theme_id
 * \return book::code value : rejected if the the is does not exist or accepted on succes
 */
book::code widget_base::set_theme(const std::string &theme_id)
{
    _theme_id_ = theme_id;
    auto const& comp = globals::colors::attr_db::theme().find(_theme_id_);
    if (comp == globals::colors::attr_db::theme().end())
    {
        book::error() << book::fn::fun << "Theme '" << color::orangered1 <<  _theme_id_ << color::reset << "' not found" << book::fn::endl;
        return book::code::rejected;
    }

    _theme_elements_ = globals::colors::attr_db::theme()[_theme_id_];
    _style_ =  globals::colors::attr_db::theme()[_theme_id_]["Widget"];
    _colors_ = _style_[globals::wstate::Active];

    book::log() << color::yellow << id() << color::reset << " theme set to '" << _style_[_state_] << " " <<  _theme_id_ << " " << color::reset << "'.";
    return book::code::accepted;
}

book::code widget_base::peek_xy(cxy xy)
{
    CHECK_BLOC

    if(!_geometry_.tolocal()[xy])
    {
        book::error() << book::fn::fun << book::code::oob << " -> " << color::red4 << xy << color::reset << " within rect:" << color::yellow << _geometry_.tolocal();
        return book::code::oob;
    }

    _iterator_ = _bloc_->begin() + xy.y * *_geometry_.width() + xy.x;
    //book::log() << book::fn::fun << color::yellow << id() << color::reset << " assigned position : " << color::red4 << xy << color::reset << ":";
    //book::out() << _iterator_->details();
    return book::code::accepted;
}

terminal::vchar::string::iterator widget_base::position(cxy xy)
{
    if(!_geometry_.goto_xy(xy))
        return {};

    peek_xy(xy);
    return _iterator_;
}


/*!
 * \brief widget_base::operator *
 * \return the pointer to the  current cell address at the internal cursor position.
 */
terminal::vchar::string::iterator widget_base::operator*()
{
    //...
    return _iterator_;
}

book::code widget_base::draw()
{
    clear();
    return book::code::done;
}


book::code widget_base::dirty(const rectangle &dirty_rect)
{
    if(!dirty_rect)
    {
        book::warning() << book::fn::fun << "attempt to merge dirty area with invalid rectangle on " << color::yellow << id() << color::reset <<book::fn::endl << "- rejected";
        return book::code::rejected;
    }
    if(!_dirty_area_)
        _dirty_area_ = dirty_rect;
    else
        _dirty_area_ |= dirty_rect;

    return book::code::accepted;
}

widget_base::painter_dc widget_base::begin_draw(const rectangle& sub_area)
{
    return {this, sub_area };
}

void widget_base::end_draw(painter_dc &edc)
{
    dirty(edc._geometry_);
}




void widget_base::clear()
{
    CHECK_BLOC
    std::fill(_bloc_->begin(), _bloc_->end(), terminal::vchar(_colors_));
}

/*!
 * \brief Instance public widget_base::render
 * \return  book::code::done or reject if not visible.
 * \note IMPORTANT! This code is temporary. It live the duration of early dev.
 */
book::code widget_base::render()
{
    book::debug() << book::fn::fun << color::yellow << id() << color::reset << "::render() : width:" << (_iterator_ + *_geometry_.width())-_bloc_->begin();
    _dirty_area_ = {};
    if(auto* p = parent<widget_base>(); p)
        return p->dirty(_geometry_);

    std::string out{};
    terminal::cursor(_geometry_.a);
    for(int y=0; y < *_geometry_.height(); y++)
    {
        peek_xy({0,y});
        terminal::cursor({_geometry_.a.x,_geometry_.a.y+y});
        terminal::vchar::render_string(_iterator_, _iterator_ + _geometry_.dwh.w);
    }
    std::cout  << std::flush;
    return book::code::done;
}

terminal::vchar *widget_base::vc()
{
    //.. Todo CHECK valid _iterator_...
    return &(*_iterator_);
}




book::code widget_base::auto_fit(globals::anchor::value anchor_value)
{
    if(anchor_value==anchor::fixed)
    {
        _ancre_ = anchor_value;
        return book::code::done;
    }
    book::debug() << book::fn::fun << '\'' << color::yellow << id() << color::reset << "' :";
    cxy off{0,0};

    // need to separate and set a simple access to the rectangle coordinates and its components:
    auto par = parent<widget_base>();
    rectangle area; // The geometry on which this element is positionning.
    if(par)
    {
        // if(par == desktop::instance())
        //     area = terminal::_geometry_;
        // else
        area = par->_geometry_.tolocal();
    }
    else
        area = terminal::geometry();

    if(par)
        if((par->_uistyle_ & uistyle::Frame) && !(par->_uistyle_ & anchor::onframe_fit))
            off={1,1};

    book::out() << id() << " offset:" << off;
    //
    book::debug() << "placement is in this area :" << color::yellow << area << color::reset;

    auto [a,b,sz] = area.components();
    auto [ea,eb,esz] = _geometry_.components(); // this 'e'lement's geometry components

    if(_ancre_ & anchor::width_fit)
    {
        book::out() << " Resize this " << id() <<  " Geometry:" << color::blue4 << _geometry_;
        resize(ui::size{area.dwh.w - (off.x*2), *_geometry_.height()});
        _geometry_.moveat({off.x,0});
        book::out() << "fit width: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset;
    }
    else
    {
        if(_ancre_ & anchor::fit_right)
        {
            book::out() << color::yellow << id() << color::reset << " fit right:";
            _geometry_.moveat(cxy{b.x - (esz.w + off.x), eb.y});
            book::out() << _geometry_;
        }
        else
            if(_ancre_ & anchor::fit_left)
            {
                book::out() << color::yellow << id() << color::reset << " fit right:";
                _geometry_.moveat(cxy{a.x+off.x, eb.y});
                book::out() << _geometry_;
            }
        // else center....
    }

    if(_ancre_ & anchor::height_fit)
    {
        resize(ui::size{_geometry_.dwh.w,*area.height()});
        book::out() << "fit height: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset;
        _geometry_.moveat({a.x, off.y});
    }
    else
    {
        if(_ancre_ & anchor::fit_bottom)
        {

            _geometry_.moveat({_geometry_.a.x, *area.height()-off.y*2});
            book::out() << "fit bottom: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset;
        }
    }
    book::out() << "applied geometry (fit_width|fit_height only as of Oct 08 '24):" << color::yellow << id() << color::lime << _geometry_ << color::yellow;

    //...

    return book::code::done;
}

book::code widget_base::resize(size new_sz)
{
    CHECK_BLOC
    _geometry_.resize(new_sz);
    book::info() << book::fn::fun << "new geometry: " << color::yellow << _geometry_ << color::reset;
    if(!parent<widget_base>())
    {
        _bloc_->resize(new_sz.area(), terminal::vchar(_colors_));
        book::out() << " bloc reallocation done.";
    }
    return book::code::done;
}



} // namespace tux::ui
