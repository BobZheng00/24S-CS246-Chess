#pragma once

#include <type_traits>

template <class E>
inline constexpr std::underlying_type_t<E> to_underlying(E e) {
    return static_cast<std::underlying_type_t<E>>(e);
}
