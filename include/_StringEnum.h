//
// Created by stort on 10/08/2025.
//

#pragma once

#define PARENS ()
#define EXPAND(...)  EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define CASE(__case__, __code__) __case__, { __code__; }
#define STR_SWITCH(__var__, ...) EXPAND(__STR_SWITCH(__var__, __VA_ARGS__))

#define __STR_SWITCH(__var__, __case__, __code__, ...)          \
if ((__var__) == (__case__)) __code__                           \
__VA_OPT__(else __STR_SWITCH2 PARENS (__var__, __VA_ARGS__))

#define __STR_SWITCH2() __STR_SWITCH

#define STRING_ENUM(__name__, ...)                                              \
struct __name__ {                                                               \
        std::string_view value;                                                 \
                                                                                \
        /* ! MUST BE DEFAULT CONSTRUCTIBLE FOR GLAZE TO WORK ! */               \
        constexpr __name__() : value() {}                                       \
                                                                                \
        constexpr __name__(const char *v) : value(v) {}                         \
        operator std::string() const { return std::string(value); }             \
                                                                                \
        __VA_OPT__(EXPAND(__DECLARE_STRING_ENUM(__name__, __VA_ARGS__)))        \
};                                                                              \
__VA_OPT__(EXPAND(__INITIALIZE_STRING_ENUM(__name__, __VA_ARGS__)))

#define __DECLARE_STRING_ENUM(__name__, __field__, __value__, ...)      \
static const __name__ __field__;                                        \
__VA_OPT__(__DECLARE_STRING_ENUM2 PARENS (__name__, __VA_ARGS__))

#define __DECLARE_STRING_ENUM2() __DECLARE_STRING_ENUM

#define __INITIALIZE_STRING_ENUM(__name__, __field__, __value__, ...)   \
inline constexpr __name__ __name__::__field__ = __value__;              \
__VA_OPT__(__INITIALIZE_STRING_ENUM2 PARENS (__name__, __VA_ARGS__))

#define __INITIALIZE_STRING_ENUM2() __INITIALIZE_STRING_ENUM
