//
// Created by oldlonecoder on 8/19/24.
//

//#ifndef EXPRESSION_H
//#define EXPRESSION_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once

#include <tuxvision/lexer/lexer.h>
#include "tuxvision/est/bloc.h"


namespace tux::est 
{

class OOM_API expr : public bloc
{
    lexer       _lexer_;
    token_table _token_table_;
    
    std::string_view _src_{};

public:
    expr(const std::string& _id_);
    ~expr() override;

    expr& operator=(std::string_view expr_text);

    alu       exec(std::string_view _text_="");
    rem::code clear();
    const char* source_code() override;


private:
    rem::code compile();
    rem::code lex();
    node* make_node(lex_token* a_token);

};

} // est

//#endif //EXPRESSION_H
