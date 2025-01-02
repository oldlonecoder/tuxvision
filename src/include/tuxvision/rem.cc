#include <map>
#include <tuxvision/rem.h>

//#include <vector>

using tux::ui::color;
using tux::glyph;
using tux::accent_fr;

bool operator !(rem::code c)
{
    return (c != rem::code::accepted)&&
           (c != rem::code::ok)      &&
           (c != rem::code::success) &&
           (c != rem::code::done)    &&
           (c != rem::code::ready)   &&
           (c != rem::code::finish)  &&
           (c != rem::code::complete);
}

namespace rem
{
std::map<rem::type, const char*> enums_type_dictionary = {
    {rem::type::none,        "none"},
    {rem::type::err  ,       "err"},
    {rem::type::warning,     "warning"},
    {rem::type::fatal,       "fatal"},
    {rem::type::except,      "exception"},
    {rem::type::message,     "message"},
    {rem::type::output,      ""},
    {rem::type::debug,       "debug"},
    {rem::type::info,        "info"},
    {rem::type::comment,     "comment"},
    {rem::type::syntax,      "syntax error"},
    {rem::type::status,      "status"},
    {rem::type::test,        "test"},
    {rem::type::interrupted, "interrupted"},
    {rem::type::aborted,     "aborted"},
    {rem::type::segfault,    "segmentation fault"},
    {rem::type::log,         "log"}
};


std::map<rem::code, const char*> return_codes_dictionary = {
    {rem::code::ok,             "ok"},
    {rem::code::accepted,       "accepted"},
    {rem::code::success,        "success "},
    {rem::code::rejected,       "rejected"},
    {rem::code::failed,         "failed"},
    {rem::code::empty,          "empty"},
    {rem::code::full,           "full"},
    {rem::code::notempty,       "not empty"},
    {rem::code::implemented,    "implemented"},
    {rem::code::notimplemented, "not implemented"},
    {rem::code::untested,       "untested"},
    {rem::code::eof,            "end of file"},
    {rem::code::eos,            "end of stream or string"},
    {rem::code::null_ptr,       "null pointer"},
    {rem::code::notexist,       "does not exist"},
    {rem::code::exist,          "exist"},
    {rem::code::unexpected,     "unexpected"},
    {rem::code::expected,       "expected"},
    {rem::code::blocked,        "blocked"},
    {rem::code::locked,         "locked"},
    {rem::code::overflow,       "buffer overflow"},
    {rem::code::oob,            "out of boundaries"},
    {rem::code::reimplement,    "need to be re-implemented in derived instances"},
    {rem::code::done,           "done"},
    {rem::code::complete,       "completed"},
    {rem::code::finish,         "finished"},
    {rem::code::undefined,      "undefined"},
    {rem::code::ready,          "Ready"},
    {rem::code::terminate,      "terminate"},
    {rem::code::timeout,        "timeout"},
    {rem::code::divbyzero,      "division by zero"},
    {rem::code::notvisible,     "not visible"},
    {rem::code::cancel,         "cancel"},
    {rem::code::object_ptr,     "object"},
    {rem::code::object_id,      "object id"},
};


std::map<rem::action, const char*> actions_dictionary= {
    {rem::action::enter,   "enter"},
    {rem::action::leave,   "leave"},
    {rem::action::begin,   "begin"},
    {rem::action::end,     "end"},
    {rem::action::commit,  "commit"},
    {rem::action::continu, "continue"},
    {rem::action::dismiss, "dismissed"},
};


std::map<rem::fn, const char*> functions_dictionary= {

    {rem::fn::endl,           "end line"},
    {rem::fn::func,            "function"},
    {rem::fn::file,           "file"},
    {rem::fn::line,           "line"},
    {rem::fn::stamp,          "stamp"},
    {rem::fn::hour,           "hour"},
    {rem::fn::minute,         "minute"},
    {rem::fn::seconds,        "seconds"},
    {rem::fn::weekday,        "weekday"},
    {rem::fn::month,          "month name"},
    {rem::fn::dayname,        "day name"},
    {rem::fn::day,            "day"},
    {rem::fn::monthnum,       "month number"},
    {rem::fn::year,           "year"}
};




std::string to_string(rem::type ty)
{
    return enums_type_dictionary[ty];
}

std::string to_string(rem::code cod)
{
    return return_codes_dictionary[cod];
}



std::string to_string(rem::fn fn)
{
    return functions_dictionary[fn];
}

std::string to_string(rem::action ac)
{
    return actions_dictionary[ac];
}

std::map<rem::type, std::pair<tux::glyph::type, color::pair>> types_database={

    {rem::type::none,        {tux::glyph::computer,  {color::aquamarine1         , color::reset }}},
    {rem::type::err,         {tux::glyph::err1,      {color::red4                , color::reset }}},
    {rem::type::warning,     {tux::glyph::warning,   {color::yellow              , color::reset }}},
    {rem::type::fatal,       {tux::glyph::dead_head, {color::hotpink4            , color::reset }}},
    {rem::type::except,      {tux::glyph::bolt,      {color::skyblue3            , color::reset }}},
    {rem::type::message,     {tux::glyph::comment,   {color::green4              , color::reset }}},
    {rem::type::output,      {tux::glyph::notepad,   {color::grey39              , color::reset }}},
    {rem::type::debug,       {tux::glyph::bug,       {color::pink3               , color::reset }}},
    {rem::type::info,        {tux::glyph::info,      {color::lightskyblue4       , color::reset }}},
    {rem::type::comment,     {tux::glyph::comment,   {color::grey42              , color::reset }}},
    {rem::type::syntax,      {tux::glyph::handwrite, {color::lighcoreateblue     , color::reset }}},
    {rem::type::status,      {tux::glyph::admin,     {color::lightcyan3          , color::reset }}},
    {rem::type::test,        {tux::glyph::file,      {color::aquamarine3         , color::reset }}},
    {rem::type::interrupted, {tux::glyph::circle_x,     {color::khaki3              , color::reset }}},
    {rem::type::aborted,     {tux::glyph::stop,      {color::red4                , color::reset }}},
    {rem::type::segfault,    {tux::glyph::bomb,      {color::sandybrown          , color::reset }}},
    {rem::type::log,         {tux::glyph::log,       {color::orangered1          , color::reset }}}
    //...
};


std::map<rem::action      , std::pair<tux::glyph::type, color::pair>> actions_color_db = {
    {rem::action::enter   , {tux::glyph::enter        , {color::white               , color::reset}}},
    {rem::action::leave   , {tux::glyph::file         , {color::white               , color::reset}}},
    {rem::action::begin   , {tux::glyph::chronos      , {color::white               , color::reset}}},
    {rem::action::end     , {tux::glyph::dead_head    , {color::white               , color::reset}}},
    {rem::action::commit  , {tux::glyph::pencil_dr    , {color::white               , color::reset}}},
    {rem::action::continu , {tux::glyph::success      , {color::white               , color::reset}}},
       {rem::action::dismiss , {tux::glyph::arrow_dhead_right , {color::yellow         , color::reset}}},
};

std::map<rem::code, std::pair<tux::glyph::type, color::pair>> codes_database={
    {rem::code::ok,              {tux::glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::code::accepted,        {tux::glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::code::success,         {tux::glyph::success,         {color::darkgreen         ,color::reset }}},
    {rem::code::rejected,        {tux::glyph::thumb_down,      {color::hotpink4          ,color::reset }}},
    {rem::code::failed,          {tux::glyph::poop,            {color::darkgoldenrod     ,color::reset }}},
    {rem::code::empty,           {tux::glyph::arrowright,      {color::lighcoreategrey   ,color::reset }}},
    {rem::code::full,            {tux::glyph::small_dot,       {color::white             ,color::reset }}},
    {rem::code::notempty,        {tux::glyph::big_dot,         {color::white             ,color::reset }}},
    {rem::code::implemented,     {tux::glyph::arrow_head_right,{color::lime              ,color::reset }}},
    {rem::code::notimplemented,  {tux::glyph::err1,            {color::orange3           ,color::reset }}},
    {rem::code::untested,        {tux::glyph::flag,            {color::yellow            ,color::reset }}},
    {rem::code::eof,             {tux::glyph::baseline,        {color::white             ,color::reset }}},
    {rem::code::eos,             {tux::glyph::baseline,        {color::white             ,color::reset }}},
    {rem::code::null_ptr,        {tux::glyph::circle_x,        {color::hotpink4            ,color::reset }}},
    {rem::code::notexist  ,      {tux::glyph::circle_x ,       {color::white               ,color::reset }}},
    {rem::code::exist     ,      {tux::glyph::star5 ,          {color::white             ,color::reset }}},
    {rem::code::unexpected,      {tux::glyph::flag ,           {color::yellow            ,color::reset }}},
    {rem::code::expected  ,      {tux::glyph::cat ,            {color::white             ,color::reset }}},
    {rem::code::blocked   ,      {tux::glyph::unauth ,         {color::indianred3        ,color::reset }}},
    {rem::code::locked    ,      {tux::glyph::err3 ,           {color::white             ,color::reset }}},
    {rem::code::overflow  ,      {tux::glyph::dead_head,       {color::red4              ,color::reset }}},
    {rem::code::oob       ,      {tux::glyph::alien,           {color::lightcoral        ,color::reset }}},
    {rem::code::reimplement,     {tux::glyph::log,             {color::yellow            ,color::reset }}},
    {rem::code::done       ,     {tux::glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::code::complete   ,     {tux::glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::code::finish     ,     {tux::glyph::ok,              {color::lime              ,color::reset }}},
    {rem::code::undefined  ,     {tux::glyph::err3,            {color::red               ,color::reset }}},
    {rem::code::ready      ,     {tux::glyph::ok,              {color::lime              ,color::reset }}},
    {rem::code::terminate  ,     {tux::glyph::flag,            {color::hotpink4          ,color::reset }}},
    {rem::code::timeout    ,     {tux::glyph::chronos,         {color::lime              ,color::reset }}},
    {rem::code::divbyzero  ,     {tux::glyph::circle_x,        {color::red4                ,color::reset }}},
    {rem::code::notvisible ,     {tux::glyph::circle_x,        {color::yellow              ,color::reset }}},
    {rem::code::cancel     ,     {tux::glyph::circle_x,        {color::red4                ,color::reset }}},
    {rem::code::object_ptr ,     {tux::glyph::edit_pencil_br,{color::lightsteelblue3     ,color::reset }}},
    {rem::code::object_id ,      {tux::glyph::arrowright,      {color::yellow            ,color::reset }}},

};


std::map<rem::fn, std::pair<tux::glyph::type, color::pair>> functions_database={

    {rem::fn::stamp     ,      {tux::glyph::chronos  ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::func      ,      {tux::glyph::function ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::file      ,      {tux::glyph::file     ,   {color::lightcyan3        ,color::reset }}},
    {rem::fn::line      ,      {tux::glyph::baseline ,   {color::lime              ,color::reset }}},
    {rem::fn::stamp     ,      {tux::glyph::chronos  ,   {color::yellow            ,color::reset }}},
    {rem::fn::hour      ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::minute    ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::seconds   ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::weekday   ,      {tux::glyph::fabric   ,   {color::yellow            ,color::reset }}},
    {rem::fn::month     ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::dayname   ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::day       ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::monthnum  ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::year      ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}}

};





std::pair<tux::glyph::type, color::pair> type_attributes(rem::type ty)
{
    return rem::types_database[ty];
}

std::pair<tux::glyph::type, color::pair> return_code_attributes(rem::code cod)
{
    return rem::codes_database[cod];
}

std::pair<tux::glyph::type, color::pair> function_attributes(rem::fn fn)
{
    return rem::functions_database[fn];
}

std::pair<tux::glyph::type, color::pair> action_attributes(rem::action a)
{
    return rem::actions_color_db[a];
}


}
