#pragma once

/*
 * time: 2025-2-11
 * author: mabu
 */

#include "mabu_algorithm_base.h"
#include "mabu_construct.h"
#include "mabu_iterator.h"
#include "mabu_type_traits.h"
#include "mabu_utility.h"

namespace mabustl {
    // uninitialized_copy: 将[first,last)上的内容复制到以result开始的空间，返回复制结束的位置
    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::true_type) {
        return mabustl::copy(first, last, result);
    }

    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::false_type) {
        auto curr = result;
        try {
            while(first != last) {
                mabustl::construct(&*curr, *first);
                ++first;
                ++result;
            }
        } catch(...) {
            while(result != curr) {
                mabustl::destroy(&*curr);
                ++curr;
            }
        }
        return curr;
    }

    template<class InputIter, class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
        return unchecked_uninitialized_copy(first, last, result,
                                            std::is_trivially_copy_assignable<
                                                typename iterator_traits<ForwardIter>::
                                                value_type>{});
    }

    // uninitialized_copy_n: 将[first,first+n)上的内容复制到以result开始的空间，返回复制结束的位置
    template<class InputIter, class ForwardIter, class Size>
    ForwardIter unchecked_uninitialized_copy_n(InputIter first, Size n, ForwardIter result, std::true_type) {
        return mabustl::copy_n(first, n, result);
    }

    template<class InputIter, class ForwardIter, class Size>
    ForwardIter unchecked_uninitialized_copy_n(InputIter first, Size n, ForwardIter result, std::false_type) {
        auto curr = result;
        try {
            while(n >= 0) {
                mabustl::construct(&*curr, *first);
                ++first;
                ++curr;
                --n;
            }
        } catch(...) {
            while(result != curr) {
                mabustl::destroy(&*curr);
                --curr;
            }
        }

        return curr;
    }

    template<class InputIter, class ForwardIter, class Size>
    ForwardIter uninitialized_copy_n(InputIter first, Size size, ForwardIter result) {
        return unchecked_uninitialized_copy_n(first, size, result,
                                              std::is_trivially_copy_assignable<
                                                  typename iterator_traits<InputIter>::
                                                  value_type>{});
    }

    // uninitialized_fill: 在[first,last)区间内填充元素
    template<class ForwardIter, class T>
    void unchecked_uninitialized_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type) {
        return mabustl::fill(first, last, value);
    }

    template<class ForwardIter, class T>
    void unchecked_uninitialized_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type) {
        auto curr = first;
        try {
            while(curr != last) {
                mabustl::construct(&*curr, value);
                ++curr;
            }
        } catch(...) {
            while(first != curr) {
                mabustl::destroy(&*first);
                ++first;
            }
        }
    }

    // uninitialized_fill_n: 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
    template<class ForwardIter, class Size, class T>
    ForwardIter
    unchecked_uninitialized_fill_n(ForwardIter first, Size n, const T& value, std::true_type) {
        return mabustl::fill_n(first, n, value);
    }

    template<class ForwardIter, class Size, class T>
    ForwardIter
    unchecked_uninitialized_fill_n(ForwardIter first, Size n, const T& value, std::false_type) {
        auto curr = first;
        try {
            while(n > 0) {
                mabustl::construct(&*curr, value);
                --n;
                ++curr;
            }
        } catch(...) {
            while(first != curr) {
                mabustl::destroy(&*first);
                ++first;
            }
        }
        return curr;
    }

    template<class ForwardIter, class Size, class T>
    ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
        return mabustl::unchecked_uninitialized_fill_n(first, n, value,
                                                       std::is_trivially_copy_assignable<
                                                           typename iterator_traits<ForwardIter>::
                                                           value_type>{});
    }

    // uninitialized_move: 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
    template<class InputIter, class ForwardIter>
    ForwardIter unchecked_uninitialized_move(InputIter first, InputIter last, ForwardIter result, std::true_type) {
        return mabustl::move(first, last, result);
    }

    template<class InputIter, class ForwardIter>
    ForwardIter
    unchecked_uninitialized_move(InputIter first, InputIter last, ForwardIter result, std::false_type) {
        ForwardIter curr = result;
        try {
            while(first != last) {
                mabustl::construct(&*curr, mabustl::move(*first));
                ++first;
                ++curr;
            }
        } catch(...) {
            mabustl::destroy(result, curr);
        }
        return curr;
    }

    template<class InputIter, class ForwardIter>
    ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
        return mabustl::unchecked_uninitialized_move(first, last, result,
                                                     std::is_trivially_move_assignable<
                                                         typename iterator_traits<InputIter>::
                                                         value_type>{});
    }

    // uninitialized_move_n: 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninitialized_move_n(InputIter first, Size n, ForwardIter result, std::true_type) {
        return mabustl::move(first, first + n, result);
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninitialized_move_n(InputIter first, Size n, ForwardIter result, std::false_type) {
        auto curr = result;
        try {
            while(n > 0) {
                mabustl::construct(&*curr, mabustl::move(*first));
                --n;
                ++first;
                ++curr;
            }
        } catch(...) {
            while(result != curr) {
                mabustl::destroy(&*result);
                ++result;
            }
            throw;
        }
        return curr;
    }

    template<class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
        return mabustl::unchecked_uninitialized_move_n(first, n, result,
                                                       std::is_trivially_move_assignable<
                                                           typename iterator_traits<InputIter>::
                                                           value_type>{});
    }
}
