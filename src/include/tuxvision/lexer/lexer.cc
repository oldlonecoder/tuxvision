//
// Created by oldlonecoder on 24-03-08.
//

#include <tuxvision/lexer/lexer.h>
#include <tuxvision/journal/logger.h>

namespace tux
{


rem::code lexer::exec()
{
    //log::debug() << " Verify config data:" << log::eol;
    if(!m_config.production)
    {
        //log::error() << " config data is empty!" << log::eol;
        return rem::code::null_ptr;
    }
    
    //log::debug() << " Now Building the Tokens Reference Table:" << log::eol;
    m_config.production->set_reference_table();
    if(m_config.text.empty() || m_config.production->reference_table().empty())
    {
        //log::error() << " config data is empty!" << log::eol;
        return rem::code::empty;
    }

    //log::debug() << " Here is the ref_table Table Dump:"  << log::eol;
    m_config.production->dump_reference_table();

    //return rem::code::Ok;

    scanner = m_config.text;
    scanner.skip_ws();

    return loop();
}


/*!
 * @brief Virtual loop
 *
 * @return rem::code::Code...
 */
rem::code lexer::loop()
{
    log::debug() << "Before entering the loop: " << log::eol;
    log::write() << " scanner position set at (col:" << scanner()-scanner.begin() << "):{" << scanner.mark() << rem::fn::endl << "}" << log::eol;
    log::test() << " now entering in the loop:" << log::eol;
    //return rem::code::Ok;
    auto Cursor = scanner(); // Save current position.
    while(!scanner.eof() && (state != rem::code::eof))
    {
        log::debug() << " lexer::loop [Next Token]: " << scanner.mark() << log::eol;
        lex_token Token{};
        if(!tokenize(m_config.production->scan(scanner())))
        {
            log::debug() << rem::fn::func << " token not in table: " << scanner.mark() << log::eol;
            if(!!tokenize_numeric(Token))
            {
                if(scanner.eof() || (state == rem::code::eof))
                    return rem::code::accepted;
            }
        }

        if(Cursor == scanner())
        {
            log::error() << " loop: no scanning (unhandled yet or bug ;) )" << rem::fn::endl << scanner.mark() << log::eol;
            return rem::code::rejected;
        }

        Cursor = scanner();
    }

    return rem::code::accepted;

}




rem::code lexer::tokenize(lex_token Token)
{
    log::debug() << rem::fn::func << scanner.mark(); //not scanned yet that's why it doen's have line and col numbers ...yet

    static std::map<lex::type::T, rem::code (lexer::*)(lex_token&)> ScanFn = {
        {lex::type::Binary           , &lexer::tokenize_binary_operator},
        {lex::type::Null             , &lexer::tokenize_default},
        {lex::type::Unary            , &lexer::tokenize_unary_operator},
        {lex::type::Punc             , &lexer::tokenize_punctuation},
        {lex::type::Keyword          , &lexer::tokenize_keyword},
        {lex::type::Text             , &lexer::tokenize_string},
        {lex::type::Prefix           , &lexer::tokenize_prefix},
        {lex::type::Postfix          , &lexer::tokenize_postfix},
        {lex::type::LineComment      , &lexer::tokenize_cpp_comment},
        {lex::type::BlocComment      , &lexer::tokenize_comment_bloc}
    };

    rem::code r{rem::code::rejected};
    for(auto [T, Fn] : ScanFn)
    {
        if(T & Token.sem)
           return (this->*Fn)(Token);
    }
    return r;
}




rem::code lexer::tokenize_binary_operator(lex_token &newtoken)
{
    log::debug() << newtoken.details() << scanner.mark() << log::eol;
    (void)tokenize_sign_prefix(newtoken);
    newtoken.token_location = scanner.sync();
    scanner.step(static_cast<int32_t>(newtoken.token_location.length));
    push_token(newtoken); // s'occupera de mettre à jour le token et d'avancer le tscanner. ;)

    return rem::code::accepted;
}

rem::code lexer::tokenize_default(lex_token &newtoken)
{
    log::debug() << " entering with:" << (newtoken ? newtoken.details() : scanner.mark()) << log::eol;

    log::debug() << " test scannumber:" << log::eol;
    // auto r = scanner.scannumber();
    // if(!!r.first)
    // {
    //     newtoken.num_data = new lus::StrScanner::Numeric::details{R.second};
    //     newtoken.token_location = scanner.Sync();
    //     newtoken.token_location.length = newtoken.num_data->Seq.length();
    //     newtoken.token_location.begin  = newtoken.num_data->Seq.begin();
    //     newtoken.token_location.end    = newtoken.num_data->Seq.end();
    //     newtoken.Prim       =lex::type::Number;
    //     newtoken.name       = lex::lexem::Numeric;
    //     state = scanner.Step(static_cast<int32_t>(newtoken.num_data->Seq.length()));
    //     newtoken.numeric_tr();
    //     (*m_config.production) << newtoken;
    //     state = scanner.SkipWS() ? rem::code::Eof : rem::code::Ok;
    //     log::debug() << "Pushed: " << newtoken.details();
    // }
    //else
    //{
        // auto [Ic,C] = Enums::ReturnCodeAttributes(R.first);
        // log::debug() << " ScanNumber:" << C << Ic << Enums::ToString(R.first);
        // log::debug() << " test scan identifier token:";
    return tokenize_identifier(newtoken);
    //}
   // return rem::code::Accepted;
}


rem::code lexer::tokenize_unary_operator(lex_token &newtoken)
{
    newtoken.token_location = scanner.sync();
    state = scanner.step(static_cast<int32_t>(newtoken.token_location.length));
    log::debug() << newtoken.details() << log::eol;
    push_token(newtoken);
    log::debug() << "pushed: " << newtoken.details() << log::eol;
    return rem::code::accepted;
}

rem::code lexer::tokenize_punctuation(lex_token &newtoken)
{
    log::debug() << rem::fn::func << " sync'ing the scanner:" << log::eol;
    newtoken.token_location = scanner.sync();
    //newtoken.token_location.begin = scanner();
    //newtoken.token_location.end = newtoken.token_location.begin + newtoken.token_location.length-1;
    //newtoken.name    = newtoken.token_location();
    log::debug() << "pushing new [punctuation] token" << log::eol;
    state = scanner.step(static_cast<int32_t>(newtoken.token_location.length));
    push_token(newtoken);
    log::debug() << "pushed: " << rem::fn::func << newtoken.details();
    return rem::code::accepted;
}

rem::code lexer::tokenize_keyword(lex_token &newtoken)
{
    log::debug() << " sync'ing the scanner:";
    newtoken.token_location = scanner.sync();
    //newtoken.token_location.begin = scanner();
    //newtoken.token_location.end = newtoken.token_location.begin + newtoken.token_location.length-1;
    //newtoken.name    = newtoken.token_location();
    state = scanner.step(static_cast<int32_t>(newtoken.token_location.length));
    log::debug() << "pushing new [keyword] token";
    push_token(newtoken);
    log::debug() << "pushed: " << newtoken.details();
    return rem::code::accepted;
}


rem::code lexer::tokenize_string(lex_token &newtoken)
{
    return rem::code::ok;
}


rem::code lexer::tokenize_identifier(lex_token &newtoken)
{
    log::debug() << scanner(); //scanner.Mark();
    auto i = scanner.scan_identifier();
    if(!i.first) return i.first;


    log::debug() << " Fill newtoken Numeric literal Infos (scanner's LocationData -> newtoken::location_data):";
    newtoken.token_location = scanner.sync();
    newtoken.token_location.length = i.second.length();
    newtoken.token_location.begin = i.second.begin();
    newtoken.token_location.end = i.second.end() - 1;
    newtoken.name    = lex::lexem::Identifier;
    newtoken.prim    =lex::type::Id;
    newtoken.sem     =lex::type::Id|lex::type::Leaf;
    newtoken.flags = { .V = 1 };
    log::debug() << "Pushing New [Identifier] Token";
    (*m_config.production) << newtoken;
    scanner.step(static_cast<int32_t>(newtoken.token_location.length));
    scanner.skip_ws();
    log::debug() << "Pushed: " << newtoken.details();

    return rem::code::accepted;
}

rem::code lexer::tokenize_sign_prefix(lex_token &newtoken)
{
    log::debug() << rem::fn::func << ":";
    if ((newtoken.m != lex::mnemonic::Sub) && (newtoken.m != lex::mnemonic::Add))
    {
        log::write() << " rejected (not subtract or addition operator).";
        return rem::code::rejected;
    }
    
    if(!m_config.production->empty())
    {
        auto &token = m_config.production->production_table().back();
        log::write() << "prev: " << token.details(true);
        if((!token.has_type(lex::type::Binary |lex::type::Punc)) || (token.m == lex::mnemonic::ClosePar))
        {
            log::write() << " rejected: previous token is not an operator or a expression-terminating punctuation token";
            return rem::code::rejected;
        }
    }

    log::write() << " conditions met for unary prefix sign operator:";
    newtoken.prim =lex::type::Sign;
    newtoken.sem = (newtoken.sem & ~(lex::type::Postfix|lex::type::Binary)) | lex::type::Prefix |lex::type::Unary | lex::type::Operator| lex::type::Sign;
    newtoken.m = lex::mnemonic::Negative;
    newtoken.token_location.length = 1;
    newtoken.token_location.end = newtoken.token_location.begin + 1;
    return rem::code::accepted;
}



/*!
 * @brief tokenize prefix operator after verifying that the prefix-operator is well formed and does not break the arithmetic syntax rules
 * @param newtoken
 *
 * @note a * *B;
 * @return rem::code::Code
 */
rem::code lexer::tokenize_prefix(lex_token &newtoken)
{
    if ((newtoken.m == lex::mnemonic::Decr) || (newtoken.m == lex::mnemonic::Incr) || (newtoken.m == lex::mnemonic::BinaryNot))
    {
        if(production().empty())
        {
            push_token(newtoken);
            return rem::code::accepted;
        }
        if((production().back().flags.V) && (!production().back().has_type(lex::type::Operator)))
        {
            newtoken.prim   = lex::type::Postfix;
            newtoken.sem    &= ~lex::type::Prefix |lex::type::Postfix;
            if(newtoken.m   == lex::mnemonic::BinaryNot)
                newtoken.m  = lex::mnemonic::Factorial;
        }
        if(!production().back().flags.V)
            throw log::exception()[ log::syntax() << newtoken.details() << ": illegal; "];
    }
    push_token(newtoken);
    
    return rem::code::accepted;
}




rem::code lexer::tokenize_postfix(lex_token &newtoken)
{

    if((production().empty()) || (!production().back().flags.V))
        throw log::exception()[ log::syntax() << newtoken.details() << ": illegal;"];

    push_token(newtoken);
    
    return rem::code::ok;
}

rem::code lexer::tokenize_cpp_comment(lex_token &newtoken)
{
    return rem::code::ok;
}

rem::code lexer::tokenize_comment_bloc(lex_token &newtoken)
{
    return rem::code::ok;
}


rem::code lexer::tokenize_numeric(lex_token& newtoken)
{
    auto R = scanner.scan_number();
    if(!!R.first)
    {
        newtoken.num_data = tux::sscan::numeric::details{R.second};
        newtoken.token_location = scanner.sync();
        newtoken.token_location.length = newtoken.num_data.seq.length();
        newtoken.token_location.begin  = newtoken.num_data.seq.begin();
        newtoken.token_location.end    = newtoken.num_data.seq.end();
        newtoken.prim       =lex::type::Number;
        newtoken.name       = lex::lexem::Numeric;
        state = scanner.step(static_cast<int32_t>(newtoken.num_data.seq.length()));
        newtoken.numeric_tr();
        (*m_config.production) << newtoken;
        state = scanner.skip_ws() ? rem::code::eof : rem::code::ok;
        
        return rem::code::accepted;
    }
    return rem::code::rejected;
}


/*!
 * @brief
 * @param Token
 *
 * @todo Assign location data here.
 */
void lexer::push_token(lex_token &newtoken)
{
    log::debug() << " Pushing newtoken : " << rem::fn::endl  << newtoken.details() << log::eol;

    (*m_config.production) << newtoken;
    
    //scanner.Sync(newtoken.token_location.length);
    log::debug() << "SkipWS: ";
    scanner.skip_ws();
}

lex_token::list& lexer::production()
{
    return m_config.production->ref_table;
}

void lexer::update_token_location(lex_token &newtoken)
{

}


} // lex
