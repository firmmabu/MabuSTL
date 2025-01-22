#pragma once
#include "type_traits"
#include<cstddef>

/*
 * time: 2025-1-22
 * author: mabu
 */

// 工具类 包含一些常用的函数
// move forward swap等

namespace mabustl {
    // move：一个对象的资源直接移动给另一个对象，输入的对象会被强制转化为右值
    template<class T>
    // std::remove_reference 移除引用（包括引用&和右值引用&&）
    // std::remove_reference<T>::type 移除引用后的类型
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
        return static_cast<typename std::remove_reference<T>::type &&>(arg);
    }

    // forward：完美转发，保留左值或者右值这种属性
    template<class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
        return static_cast<T &&>(arg);
    }

    template<class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
        // 传入左值引用触发断言
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T &&>(arg);
    }

    // swap
    template<class T>
    void swap(T& lhs, T& rhs) {
        auto tmp(mabustl::move(lhs));
        lhs = mabustl::move(rhs);
        rhs = mabustl::move(tmp);
    }

    //  将first1到last2的值与first2开始等长度的值交换
    template<class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
        for(; first1 != last1; ++first1, (void) ++first2) {
            mabustl::swap(*first1, *first2);
        }
        return first2;
    }

    // 对数组的特化交换函数
    template<class T, size_t N>
    void swap(T (&a)[N], T (&b)[N]) {
        mabustl::swap_range(a, a + N, b);
    }

    /******************************************************************************************************************/
    // pair

    template<class T1, class T2>
    struct pair {
    public:
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

    public:
        // 默认构造函数
        template<class U1=T1, class U2=T2,
            typename=typename std::enable_if<std::is_default_constructible<U1>::value &&
                                             std::is_default_constructible<U2>::value>::type>
        constexpr pair(): first(), second() {}

        // 允许隐式转换的构造函数
        template<class U1=T1, class U2=T2,
            typename std::enable_if<std::is_copy_constructible<U1>::value &&
                                    std::is_copy_constructible<U2>::value &&
                                    std::is_convertible<const U1 &, T1>::value &&
                                    std::is_convertible<const U2 &, T2>::value, int>::type = 0>
        constexpr pair(const T1& a, const T2& b): first(a), second(b) {}

        // 不允许隐式转换的构造函数
        template<class U1=T1, class U2=T2,
            typename std::enable_if<std::is_copy_constructible<U1>::value &&
                                    std::is_copy_constructible<U2>::value &&
                                    (!std::is_convertible<const U1 &, T1>::value ||
                                     !std::is_convertible<const U2 &, T2>::value), int>::type = 0>
        explicit constexpr pair(const T1& a, const T2& b): first(a), second(b) {}

        // 复制构造函数
        pair(const pair& rhs) = default;

        pair(pair&& rhs) = default;

        // 由其他类型构造的允许隐式转换的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                std::is_convertible<Other1 &&, T1>::value &&
                std::is_convertible<Other2 &&, T2>::value, int>::type = 0>
        constexpr pair(Other1&& a, Other2&& b)
            : first(mabustl::forward<Other1>(a)),
              second(mabustl::forward<Other2>(b)) {}

        // 从其他类型构造的不允许隐式转化的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                (!std::is_convertible<Other1, T1>::value ||
                 !std::is_convertible<Other2, T2>::value), int>::type = 0>
        explicit constexpr pair(Other1&& a, Other2&& b)
            : first(mabustl::forward<Other1>(a)),
              second(mabustl::forward<Other2>(b)) {}

        // 由另一个pair构造的允许隐式转化的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, const Other1 &>::value &&
                std::is_constructible<T2, const Other2 &>::value &&
                std::is_convertible<const Other1 &, T1>::value &&
                std::is_convertible<const Other2 &, T2>::value, int>::type = 0>
        constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
              second(other.second) {}

        // 由另一个pair构造的不允许隐式转化的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, const Other1 &>::value &&
                std::is_constructible<T2, const Other2 &>::value &&
                (!std::is_convertible<const Other1 &, T1>::value ||
                 !std::is_convertible<const Other2 &, T2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
              second(other.second) {}

        // 由另一个右值pair构造的允许隐式转化的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                std::is_convertible<Other1, T1>::value &&
                std::is_convertible<Other2, T2>::value, int>::type = 0>
        constexpr pair(pair<Other1, Other2>&& other)
            : first(mabustl::forward<Other1>(other.first)),
              second(mabustl::forward<Other2>(other.second)) {}

        // 由另一个右值pair构造的不允许隐式转化的构造函数
        template<class Other1, class Other2,
            typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                (!std::is_convertible<Other1, T1>::value ||
                 !std::is_convertible<Other2, T2>::value), int>::type = 0>
        explicit constexpr pair(pair<Other1, Other2>&& other)
            : first(mabustl::forward<Other1>(other.first)),
              second(mabustl::forward<Other2>(other.second)) {}

        // 重载=
        pair& operator=(const pair& rhs) {
            if(this != rhs) {
                this->first = rhs.first;
                this->second = rhs.second;
            }

            return *this;
        }

        pair& operator=(const pair&& rhs) {
            if(this != rhs) {
                this->first = mabustl::move(rhs.first);
                this->second = mabustl::move(rhs.second);
            }

            return *this;
        }

        template<class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>& rhs) {
            if(this != rhs) {
                this->first = rhs.first;
                this->second = rhs.second;
            }

            return *this;
        }

        template<class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>&& rhs) {
            if(this != rhs) {
                this->first = mabustl::forward<Other1>(rhs.first);
                this->second = mabustl::forward<Other1>(rhs.second);
            }

            return *this;
        }

        ~pair() = default;

        void swap(pair& other) {
            if(this != other) {
                mabustl::swap(this->first, other.first);
                mabustl::swap(this->second, other.second);
            }
        }
    };

    // 重载比较运算符= > >= < <= !=
    template<class T1, class T2>
    bool operator==(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p1.first == p2.first && p1.second == p2.second;
    }

    template<class T1, class T2>
    bool operator<(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second);
    }

    template<class T1, class T2>
    bool operator<=(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p1 < p2 || p1 == p2;
    }

    template<class T1, class T2>
    bool operator>(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p2 < p1;
    }

    template<class T1, class T2>
    bool operator>=(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return p1 > p2 || p1 == p2;
    }

    template<class T1, class T2>
    bool operator!=(const pair<T1, T2>& p1, const pair<T1, T2>& p2) {
        return !(p1 == p2);
    }

    // 全局函数，交换两个pair的值
    template<class T1, class T2>
    void swap(pair<T1, T2>& p1, pair<T1, T2>& p2) {
        p1.swap(p2);
    }

    // 全局函数make_pair，让两个变量变成一个pair
    template<class T1, class T2>
    pair<T1, T2> make_pair(T1&& first, T2&& second) {
        return pair<T1, T2>(mabustl::forward<T1>(first), mabustl::forward<T2>(second));
    }
}
