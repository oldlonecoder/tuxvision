
/******************************************************************************************
 *   Copyright (C) ...,2024,2025,... by Serge Lussier                                     *
 *   serge.lussier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/


//#pragma once

#include <tuxvision/tui/widgets/threads_pool.h>




namespace tux::ui
{

#pragma region workers

ansi_parser_worker::ansi_parser_worker()
{
    //...stop here - have to go to work...:(
    _id_ = std::thread::id();
}


rem::code ansi_parser_worker::start()
{
    return rem::code::notimplemented;
}

#pragma endregion workers

// --------------------------------------------------------------------------------------------------------------------

#pragma region threads_pool

threads_pool::threads_pool(size_t num_threads): _num_threads(num_threads){}


threads_pool::~threads_pool()
{
    for (auto& w: _workers_) w.join();
    _workers_.clear();
}


void threads_pool::start()
{
    for (auto n = 0; n < _num_threads; ++n)
    {
        _workers_.emplace_back();
        _threads_.emplace_back(ansi_parser_worker());
    }
}


rem::code threads_pool::enqueue(event& ev)
{
    {
        std::unique_lock<std::mutex> lock(q_mutex);
    }
    return rem::code::notimplemented;

}


#pragma endregion threads_pool






} // namespace tux::ui
