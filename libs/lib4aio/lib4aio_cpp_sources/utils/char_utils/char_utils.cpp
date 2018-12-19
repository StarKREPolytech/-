#include <lib4aio_cpp_headers/utils/char_utils/char_utils.h>

namespace lib4aio {

    bool is_space(const char c)
    {
        return c == ' ';
    }

    bool is_line_break(const char c)
    {
        return c == '\n';
    }

    bool is_space_or_line_break(const char c)
    {
        return is_space(c) || is_line_break(c);
    }

    bool is_opening_parenthesis(const char c)
    {
        return c == '(';
    }

    bool is_closing_parenthesis(const char c)
    {
        return c == ')';
    }

    bool is_opening_brace(const char c)
    {
        return c == '{';
    }

    bool is_closing_brace(const char c)
    {
        return c == '}';
    }

    bool is_equal_sign(const char c)
    {
        return c == '=';
    }

    bool is_opening_bracket(const char c)
    {
        return c == '[';
    }

    bool is_closing_bracket(const char c)
    {
        return c == ']';
    }

    bool is_sign(const char c)
    {
        return
                c == '+'
                || c == '-'
                || c == '*'
                || c == '/'
                || c == '&'
                || c == '='
                || c == '~'
                || c == '>'
                || c == '<'
                || c == '%'
                || c == '^'
                || c == '!';
    }

    bool is_dot(const char c)
    {
        return c == '.';
    }

    bool is_colon(const char c)
    {
        return c == ':';
    }

    bool is_hyphen(const char c)
    {
        return c == '-';
    }

    bool is_semicolon(const char c)
    {
        return c == ';';
    }

    bool is_comma(const char c)
    {
        return c == ',';
    }

    bool is_single_quote(const char c)
    {
        return c == '\'';
    }

    bool is_more_sign(const char c)
    {
        return c == '>';
    }

    bool is_less_sign(const char c)
    {
        return c == '<';
    }

    bool is_plus_sign(const char c)
    {
        return c == '+';
    }

    bool is_minus_sign(const char c)
    {
        return c == '-';
    }

    bool is_multiply_sign(const char c)
    {
        return c == '*';
    }

    bool is_division_sign(const char c)
    {
        return c == '/';
    }

    bool is_mod_sign(const char c)
    {
        return c == '%';
    }

    bool is_tilde_sign(const char c)
    {
        return c == '~';
    }

    bool is_and_sign(const char c)
    {
        return c == '&';
    }

    bool is_or_sign(const char c)
    {
        return c == '|';
    }

    bool is_exclamation_point(const char c)
    {
        return c == '!';
    }

    bool is_slash(const char c)
    {
        return c == '/';
    }
}