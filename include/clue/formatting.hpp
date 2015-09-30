#ifndef CLUE_FORMATTING__
#define CLUE_FORMATTING__

#include <clue/formatting_base.hpp>
#include <clue/string_builder.hpp>

namespace clue {
namespace fmt {

// to-string formatting

template<typename T, typename Fmt>
inline ::std::string strf(const T& x, const Fmt& fmt) {
    size_t fmt_len = fmt.max_formatted_length(x);
    ::std::string s(fmt_len, '\0');
    size_t wlen = fmt.formatted_write(x, const_cast<char*>(s.data()), fmt_len + 1);
    CLUE_ASSERT(wlen <= fmt_len);
    if (wlen < fmt_len) {
        s.resize(wlen);
    }
    return ::std::move(s);
}

inline ::std::string str() {
    return ::std::string();
}

template<typename T>
inline enable_if_t<
    is_default_formattable<decay_t<T>, char>::value,
    ::std::string>
str(const T& x) {
    return strf(x, default_formatter(x));
}

template<typename T, typename Fmt>
inline ::std::string str(fmt::with_fmt_t<T, Fmt> wfmt) {
    return strf(wfmt.value, wfmt.fmt);
}

namespace details {

template<typename T>
inline void insert_to(string_builder& sb, const T& x) {
    sb << x;
}

template<typename T, typename... Rest>
inline void insert_to(string_builder& sb, const T& x, Rest&&... rest) {
    sb << x;
    insert_to(sb, ::std::forward<Rest>(rest)...);
}

}

template<typename T1, typename... Rest>
inline std::string str(const T1& x, Rest&&... rest) {
    string_builder sb;
    details::insert_to(sb, x, ::std::forward<Rest>(rest)...);
    return sb.str();
}

} // end namespace fmt
} // end namespace clue

#endif