#pragma once

/*
 * time: 2025-1-27
 * author: mabu
 */

/*
 * 数值相关算法，实现的功能有：
 * accumulate
 * adjacent_difference
 * inner_product
 * iota
 * partial_sum
 */

#include "mabu_iterator.h"

namespace mabustl {
    /*
    * *****************************************************************************************************************
    * accumulate
    * 以init为初值，从first累加(或者按照指定的二元运算符操作)到last
    * *****************************************************************************************************************
    */
    template<class InputIter, class T>
    T accumulate(InputIter first, InputIter last, T init) {
        for(; first != last; ++first) init += (*first);
        return init;
    }

    template<class InputIter, class T, class BinaryOp>
    T accumulate(InputIter first, InputIter last, T init, BinaryOp op) {
        for(; first != last; ++first) init = op(init, *first);

        return init;
    }

    /*
    * *****************************************************************************************************************
    * adjacent_difference
    * version1: 计算相邻元素的差值，结果保存到以result为起点的区间里
    * version2: 按照自定义的操作计算相邻元素，结果保存到以result为起点的区间里
    * *****************************************************************************************************************
    */
    template<class InputIter, class OutputIter>
    OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result) {
        if(first == last) return result;

        *result = *first;
        auto value = *first;
        do {
            ++first;
            ++result;
            *result = *first - value;
            value = *first;
        } while(first != last);

        return ++result;
    }

    template<class InputIter, class OutputIter, class BinaryOp>
    OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result, BinaryOp op) {
        if(first == last) return result;

        *result = *first;
        auto value = *first;
        do {
            ++first;
            ++result;
            *result = op(*first, value);
            value = *first;
        } while(first != last);

        return ++result;
    }

    /*
    * *****************************************************************************************************************
    * inner_product
    * version1: 以init为初值，计算两个区间的内积
    * version2: 自定义operator+和operator*
    * *****************************************************************************************************************
    */
    template<class InputIter1, class InputIter2, class T>
    T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init) {
        for(; first1 != last1; ++first1, ++first2) init += (*first1 * *first2);
        return init;
    }

    template<class InputIter1, class InputIter2, class T, class BinaryOp1, class BinaryOp2>
    T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init, BinaryOp1 op1, BinaryOp2 op2) {
        for(; first1 != last1; ++first1, ++first2) init = op1(init, op2(*first1, *first2));
        return init;
    }

    /*
    * *****************************************************************************************************************
    * iota
    * 填充[first,last)，以value为初值开始递增
    * *****************************************************************************************************************
    */
    template<class ForwardIter, class T>
    void iota(ForwardIter first, ForwardIter last, T value) {
        for(; first != last; ++first, ++value) *first = value;
    }

    /*
    * *****************************************************************************************************************
    * partial_sum
    * version1: 计算局部累计求和，结果保存到以result为起始的区间上
    * version2: 局部进行自定义操作，结果保存到以result为起始的区间上
    * *****************************************************************************************************************
    */
    template<class InputIter, class OutputIter>
    OutputIter partial_sum(InputIter first, InputIter last, OutputIter result) {
        if(first == last) return result;
        *result = *first;
        auto value = *first;

        do {
            ++first;
            ++result;
            value += (*first);
            *result = value;
        } while(first != last);
        return ++result;
    }

    template<class InputIter, class OutputIter, class BinaryOp>
    OutputIter partial_sum(InputIter first, InputIter last, OutputIter result, BinaryOp op) {
        if(first == last) return result;
        *result = *first;
        auto value = *first;

        do {
            ++first;
            ++result;
            value = op(value, *first);
            *result = value;
        } while(first != last);
        return ++result;
    }
}
