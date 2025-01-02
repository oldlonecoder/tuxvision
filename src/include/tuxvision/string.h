//
// Created by oldlonecoder on 8/21/24.
//

//#ifndef TUX_STRING_H
//#define TUX_STRING_H
/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge oslsier                                          *
 *   serge.oslsier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge oslsier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge oslsier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge oslsier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/



#pragma once
#include <tuxvision/rem.h>
#include <tuxvision/cadres.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <chrono>
#include <stack>

namespace tux
{

using namespace integers;
/*!
 * @brief tux string for accumulating substring using std tools such as std::instrstram; ostringstream; std::fmt.
 */
class OOM_API string final
{
    std::string _d_;
public:

    using list = std::vector<std::string>;
    using view_list = std::vector<std::string_view>;



    string() = default;
    string(const std::string& a_str);
    string(std::string_view a_str) { _d_  = a_str; }
    string(const char* a_str);
    string(const string& a_str);
    string(char a_ch);
    string(integers::u8 v);
    string(integers::u16 v);
    string(integers::u32 v);
    string(integers::u64 v);
    string(integers::i8 v);
    string(integers::i16 v);
    string(integers::i32 v);
    string(integers::i64 v);

    string(float v);

    ~string();
    string& operator=(const string& a_str);
    string& operator=(const char* a_str);
    string& operator=(char a_ch);
    string& operator=(integers::u8 v);
    string& operator=(integers::u16 v);
    string& operator=(integers::u32 v);
    string& operator=(integers::u64 v);
    string& operator=(integers::i8 v);
    string& operator=(integers::i16 v);
    string& operator=(integers::i32 v);
    string& operator=(integers::i64 v);
    string& operator=(const std::string& a_str);
    string& operator=(float v);
    string& operator=(double v);
    template<typename T> string &operator>>(T &out)
    {
        std::istringstream in(_d_); //  When string was derived from std::string ... Oops!  std::istringstream IsIn(*this);

        // if constexpr (std::is_same_v<float,T>           || std::is_same_v<double,  T> ||
        //               std::is_same_v<long double,T>     || std::is_same_v<int,     T> ||
        //               std::is_same_v<unsigned int,T>    || std::is_same_v<int8_t,  T> ||
        //               std::is_same_v<uint8_t, T>        || std::is_same_v<int16_t, T> ||
        //               std::is_same_v<uint16_t, T>       || std::is_same_v<int32_t, T> ||
        //               std::is_same_v<uint32_t, T>       || std::is_same_v<int64_t, T> ||
        //               std::is_same_v<uint64_t, T>)
        // {
        //     in >> write;
        //     return *this;
        // }
        in >> out;
        return *this;
    }

    string& operator | (color::code color);
    string& operator | (color::pair colors);
    string& operator | (const char* a_str) { _d_ += a_str; return *this; }
    string& operator | (const std::string& a_str) { _d_ += a_str; return *this; }
    string& operator | (glyph::type);
    string& operator | (accent_fr::type ac);
    string& operator | (cadre::index fri);
    string& operator | (char c) { _d_ += c; return *this;}

    std::string operator()() { return _d_; }
    operator std::string() { return _d_; }
    operator std::string() const { return _d_; }
    operator bool() { return _d_.empty(); }
    operator bool() const { return _d_.empty(); }
    std::string::iterator begin() { return _d_.begin(); }
    std::string::iterator end() { return _d_.end(); }
    string& operator *() { return *this; }

    template<typename T> string& operator << (T v)
    {
        std::ostringstream out;
        out << v;
        _d_ += out.str();
        return *this;
    }
    void clear() { _d_.clear(); }

    #pragma region tokenizer
    

    struct OOM_API word final
    {
        std::string::iterator begin{};
        std::string::iterator last{}; ///< not the std::string::end() but really the last valid character pointer int the word.
        i64         offset{-1};
        std::string operator *();
        void sync(std::string& ref) { begin=last; offset = begin-ref.begin(); };
        using list = std::vector<word>;
    };


    static constexpr const char* s_default_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";
    size_t words(string::word::list& a_list,  bool a_keep_as_word=true, const char* a_sep = string::s_default_separators);
    template<typename In> string& hex(In& d)
    {
        std::istringstream in(_d_.c_str()+2); // bypass mandatory '0x/X" 
        in >> std::hex >> d;
        return *this;
    }
private:
    bool eat(string::word& w);

#pragma endregion tokenizer

public:
    static std::string make_str(const char *B, const char *E) { return {B, E}; }
    static std::string make_str(const tux::string::list& _list);
    static std::string now(const std::string &str_fmt);
    bool empty() { return _d_.empty(); }

    template<typename T> static std::string to_binary(T _arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t *) &_arg, nbytes);
        std::string stream;
        int s = 0;
        //bool Discard = false;
        for (int x = 1; x <= nbytes; x++) {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for (int y = 7; y >= 0; y--) { // est-ce que le bit #Y est à 1 ?
                if (s >= f) {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq)
                    stream += '1';
                else
                    stream += '0';
            }
        }
        /*tableau.Clear();*/
        return stream;
    }

    static std::string bytes(std::string_view a_seq);
    static std::string bytes(std::vector<int> a_seq);

    static std::vector<std::string_view> string_view_list(int Count, char **s, int offset=0);
    static tux::string::list make_list(int argc, char** argv, int offset=0);
};


} // tux


//#endif //TUX_STRING_H
