//
// Created by storto on 10/08/2025.
//

#pragma once

#include <string>
#include <cstdint>

enum class UtfCharKind {
        Utf8  = 1,
        Utf16 = 2,
        Utf24 = 3,
        Utf32 = 4,

        _Invalid = 1
}; // enum class UtfCharKind

struct UTFChar {
        UtfCharKind kind;
        const char  *it;

}; // struct UTFChar

class Utf8Iterator {
public:
        Utf8Iterator(const char *it, const char *begin, const char *end) :
                m_begin(begin),
                m_end(end),
                m_current(_getChar(it)) {}

        auto operator++ () -> Utf8Iterator &
        {
                if (m_current.it == m_end)
                        return *this;

                m_current = _getChar(m_current.it + static_cast<ptrdiff_t>(m_current.kind));
                return *this;
        }

        auto operator++ (int) -> Utf8Iterator
        {
                const Utf8Iterator tmp = *this;
                ++*this;
                return tmp;
        }

        auto operator-- () -> Utf8Iterator &
        {
                if (m_current.it == m_begin)
                        return *this;

                m_current = _getChar(m_current.it - static_cast<ptrdiff_t>(m_current.kind));
                return *this;
        }

        auto operator-- (int) -> Utf8Iterator
        {
                const Utf8Iterator tmp = *this;
                --*this;
                return tmp;
        }

        auto operator+= (ptrdiff_t offset) -> Utf8Iterator &
        {
                m_current = _getChar(m_current.it + offset);
                return *this;
        }

        auto operator-= (ptrdiff_t offset) -> Utf8Iterator &
        {
                m_current = _getChar(m_current.it - offset);
                return *this;
        }

        auto operator== (const Utf8Iterator &other) const -> bool
        {
                return m_current.it == other.m_current.it;
        }

        auto operator!= (const Utf8Iterator &other) const -> bool
        {
                return !(*this == other);
        }

        auto operator *() const -> UTFChar
        {
                return m_current;
        }

        auto operator-> () const -> const UTFChar *
        {
                return &m_current;
        }

        auto operator- (const Utf8Iterator &other) const -> ptrdiff_t
        {
                return m_current.it - other.m_current.it;
        }

        auto operator- (const char *other) const -> ptrdiff_t
        {
                return m_current.it - other;
        }

        auto base() -> UTFChar *
        {
                return &m_current;
        }

        auto base() const -> const UTFChar *
        {
                return &m_current;
        }

private:
        const char *m_begin;
        const char *m_end;
        UTFChar    m_current;

        static auto _getKind(uint8_t c) -> UtfCharKind
        {
                if (c < 0x80)            // U+0000 – U+007F
                        return UtfCharKind::Utf8;
                if ((c & 0xE0) == 0xC0)  // U+0080 – U+07FF
                        return UtfCharKind::Utf16;
                if ((c & 0xF0) == 0xE0)  // U+0800 – U+FFFF
                        return UtfCharKind::Utf24;
                if ((c & 0xF8) == 0xF0)  // U+10000 – U+10FFFF
                        return UtfCharKind::Utf32;
                return UtfCharKind::_Invalid;
        }

        auto _getChar(const char *it) const -> UTFChar
        {
                it = std::clamp(it, m_begin, m_end);
                return {
                        .kind = _getKind(static_cast<uint8_t>(*it)),
                        .it   = it
                };
        }

}; // class Utf8Iterator

class Utf8StringView {
public:
        Utf8StringView(const std::string &s) : m_begin(s.data()), m_end(s.data() + s.size()) {}

        auto begin() const -> Utf8Iterator
        {
                return { m_begin, m_begin, m_end };
        }

        auto end() const -> Utf8Iterator
        {
                return { m_end, m_begin, m_end };
        }

private:
        const char *m_begin;
        const char *m_end;

}; // class UTF8StringView
