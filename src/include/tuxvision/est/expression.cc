//
// Created by oldlonecoder on 8/19/24.
//




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


//#pragma once



#include <tuxvision/est/expression.h>



namespace tux::est 
{


expr::expr(const std::string& _id_):est::bloc(nullptr,_id_, nullptr,nullptr){}


expr::~expr()
{
    _token_table_.production_table().clear();
}


expr& expr::operator=(std::string_view expr_text)
{
    if(!_src_.empty() || !_token_table_.production_table().empty()) clear();
    _src_ = expr_text;
    return *this;
}


rem::code expr::compile()
{
    try
    {
        if(auto r = lex(); !r) return r;

        //log::info() << log::fn::fun << log::fn::endl << " Entering parse tokens:";
        _root_ = {nullptr};
        log::test() << log::eol;
        log::write() << " testing node tree_begin(lex_token*) with valgrind leak and wr errors:";
        if(_token_table_.production_table().empty())
        {
            log::status() << " the production tokens is empty. Leaving ";
            return rem::code::empty;
        }
        // auto& token = *_token_table_.production_table().begin();
        // auto* ins = node::tree_begin(this,&token, [this](lex_token* a_token) -> node*{
        //     return make_node(a_token);
        // });
        node* tmp{nullptr};
        for(auto& token: _token_table_.production_table())
        {
            if(!_root_) 
            {   
                _root_ = node::tree_begin(this,&token, [this](lex_token* a_token)->node* {
                    return make_node(a_token);
                });

                if(!_root_) return rem::code::rejected;
                tmp = _root_;
                continue;
            }
            
            if(tmp) // true, prevent potential crash here...
                tmp = tmp->tree_input(this, &token, [this](lex_token* a_token)->node* {
                    return make_node(a_token);
                });

            if(!tmp)
                break;
            _root_ = tmp;
        }
        _root_  = _root_->close_tree();
        return rem::code::success;
    }catch(log::exception& e){
        std::cerr << "a log::exception was thrown during the execution of the lexer. Descriptions are available after the log::purge(...) call." << std::endl;
    }catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    return rem::code::ok;
}



rem::code expr::lex()
{
    _lexer_.config() =
    {
        .text = _src_,               // Yes the arithmetic expr is held into object::id()
        .production = &_token_table_
    };

    try
    {
        auto r = _lexer_.exec();
        auto [ic,a] = rem::return_code_attributes(r);
        log::status() << "lexer returns: " << a << ic  << rem::to_string(r) << color::reset << " -> ";
        _token_table_.dump_production_table();    /* code */
    }
    catch(const std::exception& e)
    {
        std::cerr << "a log::exception was thrown during the execution of the lexer. Descriptions are available after the log::purge(...) call." << std::endl;
        return rem::code::rejected;
    }
    
    return rem::code::accepted;
    
}



node *expr::make_node(lex_token *a_token)
{
    log::debug() <<  rem::fn::endl << " token:" << a_token->text() << log::eol;

    if(a_token->has_type(lex::type::Id))
    {
        log::write() << "handling identifier '" << color::yellow << a_token->text() << color::reset << "' :";
        variable* lvar = new variable(this, a_token,nullptr);
        variable* v = query_variable(a_token);
        if(v)
        {
            lvar->mem.acc = 1;
            lvar->_a_ = v->_a_;
            lvar->mem.ref = 1;
            return lvar;
        }
        add_local_variable(lvar);
        return lvar;
    }
    return new est::node(this,a_token);
    
}

alu expr::exec(std::string_view _text_)
{
    if(!_text_.empty() && _src_.empty())
        _src_ = _text_;

    if(auto r = compile(); !r) return {};
    if(_root_) 
        *_a_ = _root_->JSR();
    return *_a_;

}


rem::code expr::clear()
{
    if(_src_.empty()) return rem::code::rejected;
    _token_table_.production_table().clear(); // duh...
    return rem::code::accepted;
}


const char* expr::source_code() { return _src_.data(); }

} // namespace lus::est
