/******************************************************************************************
*   Copyright (C) ...,2024,... by Serge Lussier                                          *
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

#pragma once

#include <tuxvision/rem.h>
#include <functional>
#include <utility>

// ------- copied, intellectual proprietary to:
// https://schneegans.github.io/tutorials/2015/09/20/signal-slot :
// 0xAC6A { 44138 }
// Modified and adapted for Rem and implement in basic threads, by oldlonecoder
// ------------------------------------------------------------------

namespace tux
{



template<typename... Args> class action_delegator
{

public:
    std::string _id_{"anonymous signal"};
    integers::i16    _handle_{-1};
    using slot = std::function<rem::action(Args...)>;
    using array = std::vector<typename action_delegator::slot>;
    using iterator = typename action_delegator::array::iterator;
    using accumulator = std::vector<rem::action>;

    action_delegator() = default;
    ~action_delegator() = default;

    explicit action_delegator(std::string id_) : _id_(std::move(id_))
    {}

    action_delegator(std::string id_, typename action_delegator::accumulator &acc) : _id_(std::move(id_)), _acc(&acc)
    {}

    // Copy constructor and assignment create a new delegator.
    action_delegator(action_delegator const & other)
    {
        if(this != &other)
        {
            _slots = other._slots;
            _acc = other._acc;
            _id_ = other._id_;
        }
    }

    action_delegator &operator=(action_delegator const &other)
    {
        if (this != &other)
        {
            _slots = other._slots;
            _acc = other._acc;
            _id_ = other._id_;
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    action_delegator(action_delegator &&other) noexcept:
        _slots(std::move(other._slots)),
        _id_(std::move(other._id_)),
        _acc(other._acc)
    {}

    action_delegator &operator=(action_delegator &&other) noexcept
    {
        if (this != &other)
        {
            _slots = std::move(other._slots);
            _id_ = std::move(other._id_);
            _acc = other._acc;
        }

        return *this;
    }


    // Connects a std::function to the delegator. The returned
    // value can be used to Disconnect the function again.
    integers::i16 connect(std::function<rem::action(Args...)> const &aslot) const
    {
        _slots.push_back(aslot);
        return (_slots.size()-1) & 0x7FFF;
    }

   
    // Convenience method to Connect a member function of an
    // object to this delegator.
    template<typename T> integers::i16 connect(T *inst, rem::action(T::* fun)(Args...))
    {
        return connect([=](Args... args)
                       {
                           return (inst->*fun)(args...);
                       });
    }


    // Convenience method to Connect a const member function
    // of an object to this delegator.
    template<typename T> integers::i16 Connect(T *inst, rem::action(T::* fun)(Args...) const)
    {
        return connect([=](Args... args)
                       {
                           return (inst->*fun)(args...);
                       });
    }

    // Disconnects a previously connected function.
    void disconnect(integers::i16 id) const
    {
        if(id>=_slots.size()) return ;
        auto I = _slots.begin() + id;
        _slots.erase(I);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const
    {
        if(!_slots.empty())
            _slots.clear();
    }

    //// Calls all connected functions.
    rem::action operator()(Args... p)
    {
        if (_slots.empty())
            return rem::action::continu;
        rem::action R;
        for (auto const &fn: _slots)
        {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
            if (R != rem::action::continu) return R;
        }
        return R;
    }



    // Calls only one connected function.
    rem::action to(integers::i16 id_, Args... p)
    {
        rem::action R;
        if (id_ >= _slots.size())
        {
            R = (*_slots[id_])(p...);
            if (_acc) _acc->push_back(R);
            if (R != rem::action::continu) return R;
        }
        return R;
    }

    bool empty()
    { return _slots.empty(); }

private:

    mutable array         _slots;
    mutable accumulator * _acc{nullptr};
};



template<typename Ret=rem::action, typename ...Parameters> class delegator
{
    std::string _id_{"anonymous signal"};
    integers::i16     handle{-1};
public:

    using slot = std::function<Ret(Parameters...)>;
    using array = std::vector<typename delegator::slot>;
    using iterator = typename delegator::array::iterator;
    using accumulator = std::vector<Ret>;

    delegator() = default;

    ~delegator() = default;

    explicit delegator(std::string id_) : _id_(std::move(id_))
    {}

    delegator(std::string id_, typename delegator::accumulator &acc) : _id_(std::move(id_)), _acc(&acc)
    {}

    // Copy constructor and assignment create a new delegator.
    delegator(delegator const & /*unused*/)
    {}

    delegator &operator=(delegator const &other)
    {
        if (this != &other)
        {
            _slots = other._slots;
            _acc   = other._acc;
            _id_ = other._id_;
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    delegator(delegator &&other) noexcept:
        _slots(std::move(other._slots)),
        _id_(std::move(other._id_)),
        _acc(std::move(other._acc))
    {}

    delegator &operator=(delegator &&other) noexcept
    {
        if (this != &other)
        {
            _slots = std::move(other._slots);
            _id_ = std::move(other._id_);
            _acc = std::move(other._acc);
        }

        return *this;
    }


    // Connects a std::function to the delegator using non-class/struct lamba/functions
    integers::i16 connect(std::function<Ret(Parameters...)> const &aslot) const
    {
        _slots.push_back(aslot);
        return (_slots.size()-1) & 0x7FFF;
    }


    // Convenience method to Connect a member function of an
    // object to this delegator.
    template<typename T>  integers::i16 connect(T *inst, Ret(T::* fun)(Parameters...))
    {
        return connect([=](Parameters... Args)
                       {
                           return (inst->*fun)(Args...);
                       });
    }


    // Disconnects a previously connected function.
    void disconnect(integers::i16 id) const
    {
        if(id>=_slots.size()) return;
        auto I = _slots.begin() + id;
        _slots.erase(I);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const
    {
        if(!_slots.empty())
            _slots.clear();
    }

    //// Calls all connected functions.
    // std::pair<rem::action, Ret>
    Ret operator()(Parameters... p)
    {
        if (_slots.empty())
            return Ret();
        Ret R;
        for (auto const &fn: _slots)
        {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
        }
        return R;
    }


    bool empty()
    { return _slots.empty(); }

private:

    mutable typename delegator::array _slots;
    mutable typename delegator::accumulator *_acc{nullptr};

};
}
