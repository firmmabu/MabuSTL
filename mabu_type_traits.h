#pragma once

/*
 * time: 2025-1-20
 * author: mabu
 */

#include <type_traits>

namespace mabustl {
    template<class T, T v>
    struct m_integral_constant {
        static constexpr T value = v;
    };

    template<bool b>
    using m_bool_constant = m_integral_constant<bool, b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;

    template<class T1, class T2>
    struct pair;

    template<class T>
    struct is_pair : mabustl::m_false_type {
    };

    template<class T1, class T2>
    struct is_pair<mabustl::pair<T1, T2> > : mabustl::m_true_type {
    };
}
