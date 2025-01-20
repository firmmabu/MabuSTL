#pragma once

#include <cstddef>
#include "mabustddef.h"

namespace mabustl {
    // 一元函数的参数类型和返回值
    template<class Arg, class Result>
    struct unarg_function {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    // 二元函数的参数类型和返回值
    template<class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    // 加法
    template<class T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x + y;
        }
    };

    // 减法
    template<class T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x - y;
        }
    };

    // 乘法
    template<class T>
    struct multiplies : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x * y;
        }
    };

    // 除法
    template<class T>
    struct devides : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x / y;
        }
    };

    // 取模
    template<class T>
    struct modulus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x % y;
        }
    };

    // 否定
    template<class T>
    struct negate : public unarg_function<T, T> {
        T operator()(const T &x) const {
            return -x;
        }
    };

    // 加法证同
    template<class T>
    T identity_element(plus<T>) {
        return T(0);
    }

    // 乘法证同
    template<class T>
    T identity_element(multiplies<T>) {
        return T(1);
    }

    // 等于
    template<class T>
    struct equal_to : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x == y;
        }
    };

    // 不等于
    template<class T>
    struct not_equal_to : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x != y;
        }
    };

    // 大于
    template<class T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x > y;
        }
    };

    // 大于等于
    template<class T>
    struct greater_equal : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x >= y;
        }
    };

    // 小于
    template<class T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x < y;
        }
    };

    // 小于等于
    template<class T>
    struct less_equal : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const {
            return x <= y;
        }
    };

    // 逻辑与
    template<class T>
    struct logical_and : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x && y;
        }
    };

    // 逻辑与
    template<class T>
    struct logical_or : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const {
            return x || y;
        }
    };

    // 逻辑与
    template<class T>
    struct logical_not : public unarg_function<T, bool> {
        bool operator()(const T &x) const {
            return !x;
        }
    };

    // 证同
    template<class T>
    struct identity : public unarg_function<T, T> {
        const T &operator()(const T &x) const {
            return x;
        }
    };

    // 返回pair的第一个元素
    template<class Pair>
    struct selectFirst : public unarg_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type &operator()(const Pair &x) const {
            return x.first;
        }
    };

    // 返回pair的第二个元素
    template<class Pair>
    struct selectSecond : public unarg_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type &operator()(const Pair &x) const {
            return x.second;
        }
    };

    // 返回第一参数
    template<class Arg1, class Arg2>
    struct projectFirst : public binary_function<Arg1, Arg2, Arg1> {
        Arg1 operator()(const Arg1 &x, const Arg2 &y) const {
            return x;
        }
    };

    // 返回第二参数
    template<class Arg1, class Arg2>
    struct projectSecond : public binary_function<Arg1, Arg2, Arg2> {
        Arg2 operator()(const Arg1 &x, const Arg2 &y) const {
            return y;
        }
    };


    /*******************************************************************************************/
    // 哈希函数

    template<class Key>
    struct hash {
    };

    // 指针的哈希函数
    template<class T>
    struct hash<T *> {
        // noexcept关键字: 告诉编译器该函数不会抛异常
        size_t operator()(const T *p) const noexcept {
            // reinterpret_cast: 不进行任何检查，直接转换
            return reinterpret_cast<size_t>(p);
        }
    };

#define MABUSTL_TRIVIAL_HASH_FUNCTION(Type)\
template <> struct hash<Type>{\
    size_t operator()(Type val) const noexcept\
    {return static_cast<size_t>(val);}\
};

    MABUSTL_TRIVIAL_HASH_FUNCTION(bool)

    MABUSTL_TRIVIAL_HASH_FUNCTION(char)

    MABUSTL_TRIVIAL_HASH_FUNCTION(unsigned char)

    MABUSTL_TRIVIAL_HASH_FUNCTION(signed char)

    MABUSTL_TRIVIAL_HASH_FUNCTION(wchar_t)

    MABUSTL_TRIVIAL_HASH_FUNCTION(char16_t)

    MABUSTL_TRIVIAL_HASH_FUNCTION(char32_t)

    MABUSTL_TRIVIAL_HASH_FUNCTION(short)

    MABUSTL_TRIVIAL_HASH_FUNCTION(unsigned short)

    MABUSTL_TRIVIAL_HASH_FUNCTION(int)

    MABUSTL_TRIVIAL_HASH_FUNCTION(unsigned int)

    MABUSTL_TRIVIAL_HASH_FUNCTION(long)

    MABUSTL_TRIVIAL_HASH_FUNCTION(unsigned long)

    MABUSTL_TRIVIAL_HASH_FUNCTION(long long)

    MABUSTL_TRIVIAL_HASH_FUNCTION(unsigned long long)

#undef MABUSTL_TRIVIAL_HASH_FUNTION

    // 浮点数，逐位哈希
    inline size_t bitwise_hash(const unsigned char *first, const size_t count) {
        // 64 位
#if (_MSC_VER&&_WIN64)||((__GNUC__||__clang__)&&__SIZEOF_POINTER__==8)
        const size_t fnv_offset = 14695981039346656037ull;
        const size_t fnv_prime = 1099511628211ull;

        // 32 位
#else
        const size_t fnv_offset = 2166136261u;
        const size_t fnv_prime = 16777619u;

#endif
        size_t result = fnv_offset;
        for (size_t i = 0; i < count; ++i) {
            result ^= (size_t) first[i];
            result *= fnv_prime;
        }
        return result;
    }

    template<>
    struct hash<float> {
        size_t operator()(const float &val) const {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char *) &val, sizeof(float));
        }
    };

    template<>
    struct hash<double> {
        size_t operator()(const double &val) const {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char *) &val, sizeof(double));
        }
    };

    template<>
    struct hash<long double> {
        size_t operator()(const long double &val) const {
            return val == 0.0f ? 0 : bitwise_hash((const unsigned char *) &val, sizeof(long double));
        }
    };
}
