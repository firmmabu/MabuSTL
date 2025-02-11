#pragma once
#include "mabu_algorithm_base.h"

/*
 * time: 2025-1-27
 * author: mabu
 */

/*
 * 集合相关算法（并非stl里的set）
 * 实现的功能：
 * set_union(求并集)
 * set_intersection(求交集)
 * set_difference(求差集)
 * set_symmetric_difference(求差集的并集)
 */
namespace mabustl {
    /*
    * *****************************************************************************************************************
    * set_union
    * 求两个有序集合去重的并集
    * *****************************************************************************************************************
    */
    template<class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
                         InputIter2 first2, InputIter2 last2,
                         OutputIter result) {
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) {
                *result = *first1;
                ++first1;
            } else if(*first1 < *first2) {
                *result = *first2;
                ++first2;
            } else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }

        return mabustl::copy(first2, last2, mabustl::copy(first1, last1, result));
    }

    template<class InputIter1, class InputIter2, class OutputIter, class Compare>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
                         InputIter2 first2, InputIter2 last2,
                         OutputIter result, Compare cmp) {
        while(first1 != last1 && first2 != last2) {
            if(cmp(*first1, *first2)) {
                *result = *first1;
                ++first1;
            } else if(cmp(*first2, *first1)) {
                *result = *first2;
                ++first2;
            } else {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }

        return mabustl::copy(first2, last2, mabustl::copy(first1, last1, result));
    }

    /*
    * *****************************************************************************************************************
    * set_intersection
    * 求两个有序集合的交集
    * *****************************************************************************************************************
    */
    template<class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2,
                                OutputIter result) {
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) ++first1;
            else if(*first1 > *first2) ++first2;
            else {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }

        return result;
    }

    template<class InputIter1, class InputIter2, class OutputIter, class Compare>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2,
                                OutputIter result, Compare cmp) {
        while(first1 != last1 && first2 != last2) {
            if(cmp(*first1, *first2)) ++first1;
            else if(cmp(*first2, *first1)) ++first2;
            else {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }

        return result;
    }

    /*
    * *****************************************************************************************************************
    * set_difference
    * 求S1与S2的差集，即S1-S2
    * *****************************************************************************************************************
    */
    template<class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result) {
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            } else if(*first1 > *first2) ++first2;
            else {
                ++first1;
                ++first2;
            }
        }

        return mabustl::copy(first1, last1, result);
    }

    template<class InputIter1, class InputIter2, class OutputIter, class Compare>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2,
                              OutputIter result, Compare cmp) {
        while(first1 != last1 && first2 != last2) {
            if(cmp(*first1, *first2)) {
                *result = *first1;
                ++first1;
                ++result;
            } else if(cmp(*first2, *first1)) ++first2;
            else {
                ++first1;
                ++first2;
            }
        }

        return mabustl::copy(first1, last1, result);
    }

    /*
    * *****************************************************************************************************************
    * set_symmetric_difference
    * 求S1与S2的差集 和 S2与S1的差集 的并集，即(S1-S2)v(S2-S1)
    * *****************************************************************************************************************
    */
    template<class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2,
                                        OutputIter result) {
        while(first1 != last1 && first2 != last2) {
            if(*first1 < *first2) {
                *result = *first1;
                ++first1;
                ++result;
            } else if(*first1 > *first2) {
                *result = *first2;
                ++first2;
                ++result;
            } else {
                ++first1;
                ++first2;
            }
        }

        return mabustl::copy(first2, last2, mabustl::copy(first1, last1, result));
    }

    template<class InputIter1, class InputIter2, class OutputIter, class Compare>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2,
                                        OutputIter result, Compare cmp) {
        while(first1 != last1 && first2 != last2) {
            if(cmp(*first1, *first2)) {
                *result = *first1;
                ++first1;
                ++result;
            } else if(cmp(*first2, *first1)) {
                *result = *first2;
                ++first2;
                ++result;
            } else {
                ++first1;
                ++first2;
            }
        }

        return mabustl::copy(first2, last2, mabustl::copy(first1, last1, result));
    }
}
