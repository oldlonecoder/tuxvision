#include <tuxvision/ui/screen_desk.h>

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


/*!
 * \brief widget_base::widget_base
 * Must be the only valid contructor used for instanciating any of the parent-childrend relational objects.
 * \param _parent_obj
 * \param _id
 * \param _ui_style
 */
widget_base::widget_base(object *_parent_obj, const std::string _id, globals::uistyle::Type _ui_style):object(_parent_obj, _id),
    _uistyle_(_ui_style){}

widget_base::~widget_base(){}



/*!
 * \brief widget_base::set_geometry
 * Define the widget's geometry.
 * \param r ui::rectangle : the requested geometry values.
 * \return
 */
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
        auto_fit();
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
    return book::code::done;
}

/*!
 * \brief widget_base::set_theme
 * Query the theme id from the colors::attr_db.
 * Sets the internal _style_ and _colors_ from the query values.
 * \param theme_id
 * \return accepted or rejected if the theme name does not exist.
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

/*!
 * \brief widget_base::position
 * Explicitely sets the internal iterator at the coordinates xy - _iterator_ is then ready for read/write operations.
 * \param xy
 * \return accepted or rejected if xy is pout of boundaries of local geometry.
 */
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


/*!
 * \brief widget_base::position
 * Explicitely sets the internal iterator at the coordinates xy
 * \param xy
 * \return the value of _iterator_.
 */
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

book::code widget_base::set_anchor(globals::anchor::value _ank)
{
    _ancre_ = _ank;
    //... We have to check confilcting and confusing bits.

    return book::code::accepted;
}

/*!
 * \brief widget_base::draw
 * Self-draw or predefined draw() of the widget.
 * At this area, the base class only clears and or reset the back buffer _bloc_ with the current colors
 * \return
 */
book::code widget_base::draw()
{
    clear();
    _dirty_area_ = _geometry_.tolocal();
    for(auto* o: m_children)
    {
        if(auto* w = o->as<widget_base>(); w) w->draw();
    }
    return book::code::done;
}


/*!
 * \brief widget_base::dirty
 * Invalidating the sub-area defined by the dirty_rect rectangle. sub area is applied in Union operation [ui::rectangle::operator | (ui::rectangle rhs)]
 *  between _dirty_area_ member attribute and the given dirty_rect argument.
 * \param dirty_rect  mandatory valid rectangle.
 * \return accepted or rejeted if dirty_rect is invlalid ( nul/unset rectangle )
 */
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
    book::debug() << book::fn::fun << color::red4 << id() << color::reset << " : dirty area :" <<  color::yellow << _dirty_area_ << color::reset;
    return book::code::accepted;
}

book::code widget_base::update_child(widget_base *w)
{
    book::log() << book::fn::fun << color::lime << class_name() << color::yellow << "::" << color::lightsteelblue3 << w->id() << color::reset;
    book::out() << "dirty rect: " << w->_dirty_area_;

    if(!_dirty_area_)
        _dirty_area_ = w->_dirty_area_ + w->_geometry_.a;
    else
        _dirty_area_ |= w->_dirty_area_ + w->_geometry_.a;

    _dirty_area_ = _geometry_.tolocal() & _dirty_area_;
    if(!_dirty_area_) return book::code::rejected;

    auto p = parent<widget_base>();
    if(p)
        return p->update_child(this);
    if(is_toplevel())
    {
        screen::me()->expose_window_to_bb(this);
        return book::code::done;
    }


    throw book::exception()[
        book::except() << book::fn::fun << class_name() << "::\"" << id() << "\": no parent but also not a top-level widget."
    ];

    //return book::code::done;

}


/*!
 * \brief widget_base::begin_draw
 *
 * Creates an instance of widget_base::painter_dc object for different 'drawing' capabilities on the widget's back buffer _bloc_,
 * drawing confined by the subarea.
 *
 * \param sub_area  confined rectangle within the back buffer geometry.
 * \return new instance of widget_base::painter_dc.
 */
widget_base::painter_dc widget_base::begin_draw(const rectangle& sub_area)
{
    return {this, sub_area };
}

/*!
 * \brief widget_base::end_draw
 * Triggers dirty call.
 * \param edc
 */
void widget_base::end_draw(painter_dc &edc)
{
    dirty(edc._geometry_);
}


/*!
 * \brief widget_base::clear
 * Just clears the widget's back buffer with the current colors
 */
void widget_base::clear()
{
    CHECK_BLOC
    std::fill(_bloc_->begin(), _bloc_->end(), terminal::vchar(_colors_));
}

book::code widget_base::update()
{
    auto p = parent<widget_base>();
    if(!p && screen::me() != this)
    {
        if(!is_toplevel())
            throw book::exception()[book::except() << book::fn::fun << " non top-level widget cannot be orphan."];
        //...
    }

    // no dirty rect = this widget has nothing to update.
    if(!_dirty_area_) return book::code::ok;

    if(p) return p->update_child(this);
    return book::code::ok;
}




/*!
 * \brief Protected instance widget_base::auto_fit
 * Auto fit this widget into the parent widget according to the anchor value.
 * \param anchor_value
 * \return  accepted or rejected.
 * \note As of Oct 2024, this method is not usable. It is actually in development and experimentation states.
 */
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

            _geometry_.moveat({_geometry_.a.x, *area.height()-off.y});
            book::out() << "fit bottom: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset;
        }
    }
    book::out() << "applied geometry (fit_width|fit_height only as of Oct 08 '24):" << color::yellow << id() << color::lime << _geometry_ << color::yellow;

    //...

    return book::code::done;
}


/*!
 * \brief widget_base::resize
 * Resize the geometry of this widget.
 * \param new_sz
 * \return done.
 * \note As of Oct. 2024, there is no size checking! tuxvision is in early dev/experiments/learning and R&D.
 */
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
