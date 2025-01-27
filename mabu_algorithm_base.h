#pragma once

/*
 * time: 2025-1-27
 * author: mabu
 */

/*
* 实现的功能
* max min
* iter_swap
* copy copy_backward copy_if copy_n
* move move_backward
* equal
* fill_n fill
* lexicographical_compare
* mimatch
*/

#include <cstring>
#include "mabu_iterator.h"
#include "mabu_utility.h"

namespace mabustl {
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif

    // 比较返回a和b的较大值
    template<class T>
    const T& max(const T& a, const T& b) {
        return a > b ? a : b;
    }

    // 根据比较规则来返回a和b的较大值
    template<class T, class Compare>
    const T& max(const T& a, const T& b, Compare compare) {
        return compare(a, b) ? a : b;
    }

    // 比较返回a和b的较大值
    template<class T>
    const T& min(const T& a, const T& b) {
        return a < b ? a : b;
    }

    // 根据比较规则来返回a和b的较大值
    template<class T, class Compare>
    const T& min(const T& a, const T& b, Compare compare) {
        return compare(a, b) ? a : b;
    }

    // 将两个迭代器所指的对象交换
    template<class Iter1, class Iter2>
    void iter_swap(Iter1 iter1, Iter2 iter2) {
        mabustl::swap(*iter1, *iter2);
    }

    /*
     * *****************************************************************************************************************
     * copy 将[first,last)区间内的元素拷贝到[result,result+last-first)内
     * *****************************************************************************************************************
     */

    // input iterator版本
    template<class InputIter, class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, mabustl::input_iterator_tag) {
        for(; first != last; ++first, ++result) {
            *result = *first;
        }

        return result;
    }

    // random access iterator版本
    template<class InputIter, class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                                  mabustl::random_access_iterator_tag) {
        for(auto n = last - first; n > 0; --n, ++first, ++result) {
            *result = *first;
        }

        return result;
    }

    template<class InputIter, class OutputIter>
    OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result) {
        return unchecked_copy_cat(first, last, result, iterator_category(first));
    }

    // 对于简单类型的特化版本
    // std::remove_const 去除类型中的const，如const int变int
    // std::is_same 判断两个类型是否相同
    // std::is_trivially_copy_assignable 判断一个类型是否可以平凡复制赋值（该类型的对象可以通过简单的内存拷贝来实现赋值操作）如int，float
    // std::enable_if<Condition,T> 第一个模板参数Condition为true，则std::enable_if<Condition,T>::type为T，否则没有type属性，模板实例化失败
    template<class T, class U>
    typename std::enable_if<std::is_same<typename std::remove_const<T>::type, U>::value &&
                            std::is_trivially_copy_assignable<U>::value, U*>::type
    unchecked_copy(T* first, T* last, U* result) {
        // 要拷贝的区间的元素数量
        const auto n = static_cast<size_t>(last - first);
        if(n != 0) std::memmove(result, first, n * sizeof(U));

        return result + n;
    }

    template<class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result) {
        return unchecked_copy(first, last, result);
    }

    /*
    * ******************************************************************************************************************
    * copy_backward 将[first,last)区间内的元素拷贝到[result-last+first,result)内
    * ******************************************************************************************************************
    */

    // bidirectional iterator版本
    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                                   BidirectionalIter2 result, mabustl::bidirectional_iterator_tag) {
        while(first != last) {
            --last;
            --result;
            *result = *last;
        }

        return result;
    }

    // random access iterator版本
    template<class Iter1, class Iter2>
    Iter2 unchecked_copy_backward_cat(Iter1 first, Iter1 last, Iter2 result, mabustl::random_access_iterator_tag) {
        auto n = last - first;
        while(n > 0) {
            --result;
            --last;
            *result = *last;
            --n;
        }

        return result;
    }

    template<class Iter1, class Iter2>
    Iter2 unchecked_copy_backward(Iter1 first, Iter1 last, Iter2 result) {
        return unchecked_copy_cat(first, last, result, mabustl::iterator_category(first));
    }

    // 对于简单类型的特化版本
    // std::remove_const 去除类型中的const，如const int变int
    // std::is_same 判断两个类型是否相同
    // std::is_trivially_copy_assignable 判断一个类型是否可以平凡复制赋值（该类型的对象可以通过简单的内存拷贝来实现赋值操作）如int，float
    // std::enable_if<Condition,T> 第一个模板参数Condition为true，则std::enable_if<Condition,T>::type为T，否则没有type属性，模板实例化失败
    template<class T, class U>
    typename std::enable_if<std::is_same<typename std::remove_const<T>::type, U>::value &&
                            std::is_trivially_copy_assignable<U>::value, U*>::type
    unchecked_copy_backward(T* first, T* last, U* result) {
        const auto n = static_cast<size_t>(last - first);
        if(n != 0) {
            result -= n;
            std::memmove(result, first, n * sizeof(U));
        }

        return result;
    }

    template<class Iter1, class Iter2>
    Iter2 copy_backward(Iter1 first, Iter1 last, Iter2 result) {
        return unchecked_copy_backward(first, last, result);
    }

    /*
    * ******************************************************************************************************************
    * copy_if
    * 将[first,last)区间内满足「一元操作unary_pred」的元素拷贝到以result起始的位置上
    * ******************************************************************************************************************
    */

    template<class InputIter, class OutputIter, class UnaryPredicate>
    OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred) {
        for(; first != last; ++first) {
            if(unary_pred(*first)) {
                *result = *first;
                ++result;
            }
        }

        return result;
    }

    /*
    * ******************************************************************************************************************
    * copy_n
    * 将[first,first+n)区间内的元素拷贝到[result,result+n)内
    * 返回一个pair分别指向拷贝结束的尾部
    * ******************************************************************************************************************
    */

    // input iterator 版本
    template<class InputIter, class OutputIter, class Size>
    mabustl::pair<InputIter, OutputIter>
    unchecked_copy_n(InputIter first, Size n, OutputIter result, mabustl::input_iterator_tag) {
        while(n > 0) {
            *result = *first;
            ++result;
            ++first;
            --n;
        }

        return mabustl::make_pair(first, result);
    }

    // random access iterator 版本
    template<class InputIter, class OutputIter, class Size>
    mabustl::pair<InputIter, OutputIter>
    unchecked_copy_n(InputIter first, Size n, OutputIter result, mabustl::random_access_iterator_tag) {
        auto last = first + n;
        return mabustl::make_pair(last, mabustl::copy(first, last, result));
    }

    template<class InputIter, class OutputIter, class Size>
    mabustl::pair<InputIter, OutputIter> copy_n(InputIter first, Size n, OutputIter result) {
        return mabustl::unchecked_copy_n(first, n, result, mabustl::iterator_category(first));
    }

    /*
    * ******************************************************************************************************************
    * move
    * 将[first,last)区间内的元素移动到[result,result+last-first)内
    * ******************************************************************************************************************
    */

    // input iterator 版本
    template<class InputIter, class OutputIter>
    OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result, mabustl::input_iterator_tag) {
        while(first != last) {
            *result = mabustl::move(*first);
            ++result;
            ++first;
        }

        return result;
    }

    // random access iterator 版本
    template<class InputIter, class OutputIter>
    OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
                                  mabustl::random_access_iterator_tag) {
        auto n = last - first;
        while(n > 0) {
            *result = mabustl::move(*first);
            ++first;
            ++result;
            --n;
        }

        return result;
    }

    template<class InputIter, class OutputIter>
    OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result) {
        return unchecked_move_cat(first, last, result, mabustl::iterator_category(first));
    }

    // 对于简单类型的特化版本
    // std::remove_const 去除类型中的const，如const int变int
    // std::is_same 判断两个类型是否相同
    // std::is_trivially_copy_assignable 判断一个类型是否可以平凡复制赋值（该类型的对象可以通过简单的内存拷贝来实现赋值操作）如int，float
    // std::enable_if<Condition,T> 第一个模板参数Condition为true，则std::enable_if<Condition,T>::type为T，否则没有type属性，模板实例化失败
    template<class T, class U>
    typename std::enable_if<std::is_same<typename std::remove_const<T>::type, U>::value &&
                            std::is_trivially_move_assignable<U>::value, U*>::type
    unchecked_move(T* first, T* last, U* result) {
        const auto n = static_cast<size_t>(last - first);
        if(n != 0) std::memmove(result, first, n * sizeof(U));
        return result + n;
    }

    template<class InputIter, class OutputIter>
    OutputIter move(InputIter first, InputIter last, OutputIter result) {
        return unchecked_move(first, last, result);
    }

    /*
    * ******************************************************************************************************************
    * move_backward
    * 将[first,last)区间内的元素移动到[result+last-first,result)内
    * ******************************************************************************************************************
    */

    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                                   BidirectionalIter2 result, mabustl::bidirectional_iterator_tag) {
        while(first != last) {
            --result;
            --last;
            *result = mabustl::move(*last);
        }
        return result;
    }

    // random_access_iterator_tag 版本
    template<class RandomIter1, class RandomIter2>
    RandomIter2 unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
                                            RandomIter2 result, mabustl::random_access_iterator_tag) {
        auto n = last - first;
        while(n > 0) {
            --result;
            --last;
            *result = mabustl::move(*last);
            --n;
        }

        return result;
    }

    template<class Iter1, class Iter2>
    Iter2 unchecked_move_backward(Iter1 first, Iter1 last, Iter2 result) {
        return unchecked_move_backward_cat(first, last, result, iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template<class Tp, class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
                            std::is_trivially_move_assignable<Up>::value, Up*>::type
    unchecked_move_backward(Tp* first, Tp* last, Up* result) {
        const auto n = static_cast<size_t>(last - first);
        if(n != 0) {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }

    template<class Iter1, class Iter2>
    Iter2 move_backward(Iter1 first, Iter1 last, Iter2 result) {
        return unchecked_move_backward(first, last, result);
    }

    /*
    * ******************************************************************************************************************
    * equal
    * 比较一个序列中[first1,last1)与第二个序列中[first2,first2+last-first1)是否相等
    * ******************************************************************************************************************
    */

    template<class InputIter, class OutputIter>
    bool equal(InputIter first1, InputIter last1, OutputIter first2) {
        while(first1 != last1) {
            if(*first1 != *first2) return false;
            ++first1;
            ++first2;
        }

        return true;
    }

    template<class InputIter, class OutputIter, class Compare>
    bool equal(InputIter first1, InputIter last1, OutputIter first2, Compare compare) {
        while(first1 != last1) {
            if(!compare(*first1, *first2)) return false;
            ++first1;
            ++first2;
        }

        return true;
    }

    /*
    * ******************************************************************************************************************
    * fill_n
    * 从first开始填充n个位置的值
    * ******************************************************************************************************************
    */
    template<class Iter, class Size, class T>
    Iter unchecked_fill_n(Iter iter, Size n, const T& value) {
        while(n > 0) {
            *iter = value;
            ++iter;
            --n;
        }

        return iter;
    }

    // one-byte的特化版本
    // std::is_integral<T>::value 检测T类型是否为整型（bool short char int long longlong）
    template<class T, class Size, class U>
    typename std::enable_if<std::is_integral<T>::value &&
                            sizeof(T) == 1 &&
                            !std::is_same<T, bool>::value &&
                            std::is_integral<U>::value &&
                            sizeof(U) == 1, T*>::type
    unchecked_fill_n(T* first, Size n, U value) {
        if(n > 0) std::memset(first, (unsigned char) value, (size_t) n);

        return first + n;
    }

    template<class T, class Size, class U>
    T fill_n(T first, Size n, const U& value) {
        return unchecked_fill_n(first, n, value);
    }

    /*
    * ******************************************************************************************************************
    * fill
    * 填充[first,last)区间内的所有位置
    * ******************************************************************************************************************
    */
    template<class ForwardIter, class T>
    void fill_cat(ForwardIter first, ForwardIter last, const T& value, mabustl::forward_iterator_tag) {
        while(first != last) {
            *first = value;
            ++first;
        }
    }

    template<class RandomIter, class T>
    void fill_cat(RandomIter first, RandomIter last, const T& value, mabustl::random_access_iterator_tag) {
        mabustl::fill_n(first, last - first, value);
    }

    template<class Iter, class T>
    void fill(Iter first, Iter last, const T& value) {
        mabustl::fill_cat(first, last, value, mabustl::iterator_category(first));
    }

    /*
    * ******************************************************************************************************************
    * lexicographical_compare
    * 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
    * (1)如果第一序列的元素较小，返回 true ，否则返回 false
    * (2)如果到达 last1 而尚未到达 last2 返回 true
    * (3)如果到达 last2 而尚未到达 last1 返回 false
    * (4)如果同时到达 last1 和 last2 返回 false
    * ******************************************************************************************************************
    */
    template<class InputIter1, class InputIter2>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2) {
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) return true;
            if(*first1 < *first2) return false;

            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 != last2;
    }

    template<class InputIter1, class InputIter2, class Compare>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2,
                                 Compare compare) {
        while(first1 != last1 && first2 != last2) {
            if(compare(*first1, *first2)) return true;
            if(compare(*first2, *first1)) return false;

            ++first1;
            ++first2;
        }

        return first1 == last1 && first2 != last2;
    }

    // 针对const unsigned char*的特化版本
    bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
                                 const unsigned char* first2, const unsigned char* last2) {
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        const auto result = std::memcmp(first1, first2, mabustl::min(len1, len2));

        return result != 0 ? result < 0 : len1 < len2;
    }

    /*
    * ******************************************************************************************************************
    * mismatch
    * 平行比较两个序列，找到第一处不匹配的元素，返回一对迭代器，分别指向两个序列中不匹配的元素
    * ******************************************************************************************************************
    */

    template<class InputIter1, class InputIter2>
    mabustl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
        while(first1 != last1 && *first1 == *first2) {
            ++first1;
            ++first2;
        }

        return mabustl::pair<InputIter1, InputIter2>(first1, first2);
    }

    template<class InputIter1, class InputIter2, class Compare>
    mabustl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2,
                                                   Compare compare) {
        while(first1 != last1 && compare(*first1, *first2)) {
            ++first1;
            ++first2;
        }

        return mabustl::pair<InputIter1, InputIter2>(first1, first2);
    }
}
