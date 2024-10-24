#include <tuxvision/ui/widget.h>

namespace tux::ui
{


#define CHECK_BLOC \
if(!_bloc_)\
    throw book::exception() [ book::except() << book::fn::fun << book::code::null_ptr << " undefined backbuffer on: "\
   << color::lightsteelblue << class_name()\
   << color::reset <<"::"\
   << color::yellow << id()\
];



using namespace globals;


widget::widget()
{

}


/*!
 * \brief widget::widget
 * Must be the only valid contructor used for instanciating any of the parent-childrend relational objects.
 * \param _parent_obj
 * \param _id
 * \param _ui_style
 */
widget::widget(object *_parent_obj, const std::string _id, globals::uistyle::Type _ui_style):object(_parent_obj, _id),
    _uistyle_(_ui_style){}

widget::~widget(){}



/*!
 * \brief widget::set_geometry
 * Define the widget's geometry.
 * \param r ui::rectangle : the requested geometry values.
 * \return
 */
book::code widget::set_geometry(const rectangle &r)
{
    book::info() << color::yellow << id() << color::reset << " requested geometry:" << r << book::fn::endl;
    if(!r)
    {
        book::error() << book::fn::fun << book::code::null_ptr << " - " << color::yellow << id() << " invalid rectangle!" << book::fn::endl;
        return book::code::rejected;
    }
    _geometry_ = r;
    if(auto p = parent<widget>(); p)
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
        book::out() << color::lightsteelblue <<  class_name() << color::grey100 << "::" << color::yellow << id() << color::reset << " is toplevel widget, owns back_buffer" << book::fn::endl;
    }
    //_bkcrs_ =
    _iterator_ = _bloc_.get()->begin();
    if(!(_ancre_ & globals::anchor::fixed) && (_ancre_!=0))
       auto_fit();

    book::out() << color::yellow << id() << color::reset << " assisgned geometry:" << _geometry_ << book::fn::endl;
    return book::code::done;
}

/*!
 * \brief widget::set_theme
 * Query the theme id from the colors::attr_db.
 * Sets the internal _style_ and _colors_ from the query values.
 * \param theme_id
 * \return accepted or rejected if the theme name does not exist.
 */
book::code widget::set_theme(const std::string &theme_id)
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
 * \brief widget::position
 * Explicitely sets the internal iterator at the coordinates xy - _iterator_ is then ready for read/write operations.
 * \param xy
 * \return accepted or rejected if xy is pout of boundaries of local geometry.
 */
book::code widget::peek_xy(cxy xy)
{
    CHECK_BLOC

    if(!_geometry_.tolocal()[xy])
    {
        book::error() << book::fn::fun << book::code::oob << " -> " << color::red4 << xy << color::reset << " within rect:" << color::yellow << _geometry_.tolocal();
        return book::code::oob;
    }

    if(auto p = parent<widget>(); p)
    {
        auto pxy = xy + _geometry_.a;
        book::debug() << book::fn::fun;
        book::out() << color::yellow << id() << color::yellow3 << " peek at topleft: " << color::red4 << _geometry_.top_left() << color::reset << " + xy: " << color::red4 << pxy;
        return p->peek_xy(pxy);
    }
    else
        _iterator_ = _bloc_->begin() + xy.y * *_geometry_.width() + xy.x;
    book::debug() << color::lime << id() << color::grey100 << "::" << color::lightsteelblue3 << "peek_xy" << color::grey100 << "(" << color::red4 << xy << color::grey100 << ").";
    return book::code::accepted;
}


/*!
 * \brief widget::position
 * Explicitely sets the internal iterator at the coordinates xy
 * \param xy
 * \return the value of _iterator_.
 */
terminal::vchar::string::iterator widget::at(cxy xy)
{
    if(!_geometry_.goto_xy(xy))
        return {};

    peek_xy(xy);
    return _iterator_;
}

terminal::vchar::string::iterator widget::operator[](cxy xy){ return at(xy); }



book::code widget::set_anchor(globals::anchor::value _ank)
{
    _ancre_ = _ank;
    if(_bloc_ && _geometry_)
        auto_fit();
    //... We have to check confilcting and confusing bits.

    return book::code::accepted;
}

book::code widget::show(globals::wstate::Type st)
{
    _state_ |= st;

    book::debug() << book::fn::fun;
    book::out() << id() << " [" << _geometry_.a << "]";
    render();
    return book::code::ok;
}

/*!
 * \brief widget::draw
 * Self-draw or predefined draw() of the widget.
 * At this area, the base class only clears and or reset the back buffer _bloc_ with the current colors
 * \return
 */
book::code widget::draw()
{
    clear();
    _dirty_area_ = _geometry_.tolocal();
    for(auto* o: m_children)
        if(auto* w = o->as<widget>(); w)
            w->draw();

    return book::code::done;
}


/*!
 * \brief widget::dirty
 * Invalidating the sub-area defined by the dirty_area rectangle. 'sub' area is applied using Union operation (ui::rectangle::operator | (ui::rectangle rhs))
 *  between _dirty_area_ member attribute and the given dirty_rect argument.
 * \param dirty_rect  mandatory valid rectangle.
 * \return accepted or rejeted if dirty_rect is invlalid ( nul/unset rectangle )
 */
book::code widget::dirty(const rectangle &dirty_rect)
{
    book::debug() << book::fn::fun;
    book::out() << color::lime << class_name() << "::" << id() << color::reset
                << " dirty_rect" << color::yellow  << dirty_rect;

    if(!dirty_rect)
    {
        book::error() << book::fn::fun << "(internal) attempt to merge invalidate an area with invalid rectangle on " << color::yellow << id() << color::reset <<book::fn::endl << "- rejected";
        return book::code::rejected;
    }

    if(!_dirty_area_)
        _dirty_area_ = dirty_rect;
    else
        _dirty_area_ |= dirty_rect; // merge/update this _dirty_area_ rectangle with our child 's _dirty_area_.

    if(_dirty_area_ = _dirty_area_ & _geometry_.tolocal(); !_dirty_area_)
        return book::code::rejected;


    book::debug() << book::fn::fun << color::red4 << class_name() << "::" << id() << color::reset << " computed dirty area: " <<  color::yellow << _dirty_area_ << color::reset;

    if(auto p = parent<widget>(); p != nullptr)
    {
        book::out() << color::yellow << "signal parent widget '" << color::lime << p->id() << color::reset << "' :";
        return p->dirty(_dirty_area_+_geometry_.a);
    }

   // book::warning() << color::red4 << id() << " --> I don't know what to do from here..." << color::reset;
    // No prent widget :
    // 1- We are the screen's desktop root widget. -> this method addresses the screen's widget back buffer.
    //                                                 - We must signal (to self) that the screen's back buffer ( 2nd back buffer) needs to be updated with the root widget's dirty back buffer.
    //                                                 - Be ready to 'flush'/commit to the terminal console screen, with the screen's dirty area

    // 2- We are a top-level widget ( window )     -> we simply accept and leave here.

    return book::code::accepted;
}



/*!
 * \brief widget::update_child
 *      Updates merging child widget dirty area with this dirty area..
 * \param w
 * \return rejected if invalid computed rectangle ( requested area not visible within this geometry, or child widget has no current dirty area to update.).
 */
book::code widget::update_child(widget *w)
{
    if(!w->_dirty_area_) return book::code::rejected;
    return dirty(w->_dirty_area_);
}



/*!
 * \brief widget::render
 *          Temporary function to render this widget on the screen console.
 * \return
 */
book::code widget::render()
{
    if(!(_state_ & globals::wstate::Visible))
    {
        book::info() << book::fn::fun << id() << " is not visible.. - rendering request rejected";
        return book::code::rejected;
    }

    for(int y = 0; y < _geometry_.height(); y++)
    {
        peek_xy({0,y});
        terminal::cursor(_geometry_.a + cxy{0,y});
        terminal::vchar::render_string(_iterator_, _iterator_ + *_geometry_.width());
    }
    std::cout << std::flush;

    return book::code::done;
}


/*!
 * \brief widget::begin_draw
 *
 * Creates an instance of widget::painter_dc object for different 'drawing' capabilities on the widget's back buffer _bloc_,
 * drawing confined by the subarea.
 *
 * \param sub_area  confined rectangle within the back buffer geometry.
 * \return new instance of widget::painter_dc.
 */
widget::painter_dc widget::begin_draw(const rectangle& sub_area)
{
    return {this, sub_area };
}

/*!
 * \brief widget::end_draw
 * Triggers dirty call.
 * \param edc
 */
void widget::end_draw(painter_dc &edc)
{
    dirty(edc._geometry_);
}


/*!
 * \brief widget::clear
 * Just clears the widget's back buffer with the current colors
 */
void widget::clear()
{
    CHECK_BLOC

    book::debug() << book::fn::fun;
    book::out() << color::grey100 << class_name() << color::lightcyan3 << '[' << color::lightsteelblue3 <<  id() << color::lightcyan3 << ']' << _colors_() << " >>colors<< ;" << color::reset;
    std::fill(_bloc_->begin(), _bloc_->end(), terminal::vchar(_colors_));

}

book::code widget::update()
{
    auto p = parent<widget>();

    // no dirty rect = this widget has nothing to update.
    if(!_dirty_area_) return book::code::ok;

    if(p) return p->update_child(this);
    return book::code::ok;
}




/*!
 * \brief Protected instance widget::auto_fit
 * Auto fit this widget into the parent widget according to the anchor value.
 * \param anchor_value
 * \return  accepted or rejected.
 * \note As of Oct 2024, this method is not usable. It is actually in development and experimentation states.
 */
book::code widget::auto_fit(globals::anchor::value anchor_value)
{

    if((!_ancre_ && !anchor_value) || !_geometry_)
    {
        book::info() << book::fn::fun;
        book::out() << class_name() << "->[" << color::lime << id() << color::reset << "] has no anchor, reject auto_fit()...";
        return book::code::rejected;
    }

    book::debug() << book::fn::fun << '\'' << color::yellow << id() << color::reset << "' anchor:" << std::format("{:<04X}", _ancre_);
    cxy off{0,0};
    if(anchor_value)
        _ancre_ = anchor_value;

    // need to separate and set a simple access to the rectangle coordinates and its components:
    auto par = parent<widget>();
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

    book::out() << id() << " offset:" << off << book::fn::endl;
    //
    book::debug() << "placement is within this area :" << color::yellow << area << color::reset << book::fn::endl;

    auto [a,b,sz] = area.components();
    auto [ea,eb,esz] = _geometry_.components(); // this 'e'lement's geometry components

    if(_ancre_ & anchor::width_fit)
    {
        book::out() << " Resize this " << id() <<  " Geometry:" << color::blue4 << _geometry_ << book::fn::endl;;
        resize(ui::size{sz.w - (off.x*2), *_geometry_.height()});
        _geometry_.moveat({off.x,0});
        book::out() << "fit width: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset << book::fn::endl;
    }
    else
    {
        if(_ancre_ & anchor::fit_right)
        {
            book::out() << color::yellow << id() << color::reset << " fit right:" << book::fn::endl;
            _geometry_.moveat(cxy{sz.w - (esz.w-1 + off.x), eb.y});
            book::out() << _geometry_;
        }
        else
            if(_ancre_ & anchor::fit_left)
            {
                book::out() << color::yellow << id() << color::reset << " fit right:" << book::fn::endl;
                _geometry_.moveat(cxy{a.x+off.x, eb.y});
                book::out() << _geometry_ << book::fn::endl;
            }
        // else center....
    }

    if(_ancre_ & anchor::height_fit)
    {
        resize(ui::size{_geometry_.dwh.w,*area.height()});
        book::out() << "fit height: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset << book::fn::endl;
        _geometry_.moveat({a.x, off.y});
    }
    else
    {
        if(_ancre_ & anchor::fit_bottom)
        {

            _geometry_.moveat({_geometry_.a.x, *area.height()-off.y});
            book::out() << "fit bottom: " << color::yellow << id() << color::reset <<"::_geometry_: " << color::hotpink4 << _geometry_ << color::reset << book::fn::endl;
        }
    }
    book::out() << "applied geometry : " << color::yellow << id() << ' '  << color::lime << _geometry_ << color::yellow << book::fn::endl;

    //...

    return book::code::done;
}


/*!
 * \brief widget::resize
 * Resize the geometry of this widget.
 * \param new_sz
 * \return done.
 * \note As of Oct. 2024, there is no size checking! tuxvision is in early dev/experiments/learning and R&D.
 */
book::code widget::resize(size new_sz)
{
    CHECK_BLOC
    _geometry_.resize(new_sz);
    book::info() << book::fn::fun << "new geometry: " << color::yellow << _geometry_ << color::reset;
    if(is_toplevel())
    {
        _bloc_->resize(new_sz.area(), terminal::vchar(_colors_));
        book::out() << " bloc reallocation done.";
    }
    return book::code::done;
}



} // namespace tux::ui
