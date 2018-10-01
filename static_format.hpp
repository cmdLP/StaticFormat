/**\
**
**  Copyright cmdLP
**
**  read LICENSE.md
**
**  static_format library
**
\**/

#pragma once

namespace static_format { // overview

    inline namespace literals {

            template<typename t_Char, t_Char... t_chars>
        inline constexpr auto operator""_format();

    };

};


#include <string>
#include <sstream>

namespace static_format { // implementation

        template<typename t_Char, int t_length>
    struct opt_str_type_assoc {
        using type = t_Char[t_length + 1]; // null-terminated c-string, as fast as c-string-literals
    };

        template<typename t_Char>
    struct opt_str_type_assoc<t_Char, 1> {
        using type = t_Char;
    };

        template<typename t_Char>
    struct opt_str_type_assoc<t_Char, 0> {
        struct type {
                template<typename t>
            friend inline t& operator<<(t& ss, type self) { return ss; };
        };
    };

        template<typename t_Char, int t_length>
    using opt_str_type = typename opt_str_type_assoc<t_Char, t_length>::type;

        template<typename t_Char, t_Char... t_chars>
    struct string_constant {
        static const std::basic_string<t_Char> value;
        static const opt_str_type<t_Char, sizeof...(t_chars)> value_opt;
    };

        template<typename t_Char, t_Char... t_chars>
    const opt_str_type<t_Char, sizeof...(t_chars)> string_constant<t_Char, t_chars...>::value_opt {t_chars...};

        template<typename t_Char, t_Char first, typename t_rest>
    struct push_front_str {};

        template<typename t_Char, t_Char t_first, t_Char... t_rest>
    struct push_front_str<t_Char, t_first, string_constant<t_Char, t_rest...>>{
        using type = string_constant<t_Char, t_first, t_rest...>;
    };

        template<typename t_Char, t_Char t_first, typename t_rest>
    using push_front_str_t = typename push_front_str<t_Char, t_first, t_rest>::type;

    enum Align { LEFT, RIGHT, CENTER };

    struct FinalPart {
            template<typename t_Stream>
            __attribute__((always_inline))
        static inline void format(t_Stream& ss) {
        };
    };

        template<int t_align, int t_width, typename t_rest>
    struct InsertPart {
            template<typename t_Stream, typename t_First, typename... t_Types>
        static inline void format(t_Stream& ss, t_First first, t_Types... rest) {
            t_rest::format(ss << first, rest...);
        };
    };

        template<typename t_strConst, typename t_rest>
    struct StringPart {
            template<typename t_Stream, typename... t_Types>
        static inline void format(t_Stream& ss, t_Types... rest) {
            t_rest::format(ss << t_strConst::value_opt, rest...);
        };
    };

        template<typename t_Char, t_Char... t_chars>
    struct parse_format_rec {
        using type = StringPart<string_constant<t_Char, t_chars...>, FinalPart>;
    };

        template<typename t_Char, t_Char t_first, t_Char... t_rest>
    struct parse_format_rec<t_Char, t_first, t_rest...> {
        using follower = parse_format_rec<t_Char, t_rest...>;
        using string_rest = push_front_str_t<t_Char, t_first, typename follower::string_rest>;
        using type = typename follower::type;
    };

        template<typename t_Char>
    struct parse_format_rec<t_Char> {
        using type = FinalPart;
        using string_rest = string_constant<t_Char>;
    };

        template<typename t_Char, t_Char... t_chars>
    struct parse_format {
        using follower = parse_format_rec<t_Char, t_chars...>;
        using type = StringPart<typename follower::string_rest, typename follower::type>;
    };

        template<typename t_Char, t_Char... t_chars>
    using parse_format_t = typename parse_format<t_Char, t_chars...>::type;


    #define parse_format_spec(m_Char) \
         \
            template<m_Char... t_rest> \
        struct parse_format_rec<m_Char, (m_Char)'{', (m_Char)'}', t_rest...> { \
            using string_rest = string_constant<m_Char>; \
            using type = InsertPart<0, 0, parse_format_t<m_Char, t_rest...>>; \
        }; \
         \
            template<m_Char... t_rest> \
        struct parse_format_rec<m_Char, (m_Char)'{', (m_Char)'{', t_rest...> { \
            using follower = parse_format_rec<m_Char, t_rest...>; \
            using type = typename follower::type; \
            using string_rest = push_front_str_t<m_Char, (m_Char)'{', typename follower::string_rest>; \
        }; \
         \
            template<m_Char... t_rest> \
        struct parse_format_rec<m_Char, (m_Char)'}', (m_Char)'}', t_rest...> { \
            using follower = parse_format_rec<m_Char, t_rest...>; \
            using type = typename follower::type; \
            using string_rest = push_front_str_t<m_Char, (m_Char)'}', typename follower::string_rest>; \
        };


        parse_format_spec(char);
        parse_format_spec(wchar_t);
        parse_format_spec(char16_t);
        parse_format_spec(char32_t);

    #undef parse_format_spec


        template<typename t_Char, typename t_FormatPart>
    struct Formatter {
            template<typename... t_Types>
        std::basic_string<t_Char> operator()(t_Types... args) {
            std::basic_stringstream<t_Char> ss;
            t_FormatPart::format(ss, args...);
            return ss.str();
        };
    };

    inline namespace literals {

            template<typename t_Char, t_Char... t_chars>
        inline constexpr auto operator""_format() {
            return Formatter<t_Char, parse_format_t<t_Char, t_chars...>>{};
        };

    };

};
