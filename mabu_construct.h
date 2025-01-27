#pragma once

/*
 * time: 2025-1-27
 * author: mabu
 */

#include <new>
#include "mabu_iterator.h"
#include "mabu_type_traits.h"
#include "mabu_utility.h"

// 禁用MSVC的4100warning
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

namespace mabustl {
    // construct 创建对象

    template<class T>
    void construct(T* ptr) {
        ::new((void*) ptr) T();
    }

    template<class T1, class T2>
    void construct(T1* ptr, const T2& value) {
        ::new((void*) ptr) T1(value);
    }

    template<class T, class... Args>
    void construct(T* ptr, Args&&... args) {
        ::new((void*) ptr) T(mabustl::forward<Args>(args)...);
    }

    // destroy 析构对象

    template<class T>
    void destroy_one(T*, std::true_type) {}

    template<class T>
    void destroy_one(T* ptr, std::false_type) {
        if(ptr != nullptr) ptr->~T();
    }

    template<class ForwardIter>
    void destroy_cat(ForwardIter iter1, ForwardIter iter2, std::true_type) {}

    template<class ForwardIter>
    void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
        while(first != last) {
            // 这里应该是有问题
            //destroy(&*first);

            destroy_one(&*first, std::false_type{});
            ++first;
        }
    }

    template<class T>
    void destroy(T* ptr) {
        destroy_one(ptr, std::is_trivially_copy_assignable<T>{});
    }

    template<class Iterator>
    void destroy(Iterator first, Iterator last) {
        destroy_cat(first, last, std::is_trivially_constructible<typename iterator_traits<Iterator>::value_type>{});
    }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
