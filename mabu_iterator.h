#pragma once

/*
 * time: 2025-1-20
 * author: mabu
 */

#include <cstddef>
#include "mabu_type_traits.h"

namespace mabustl {
    struct input_iterator_tag {};

    struct output_iterator_tag {};

    struct forward_iterator_tag : public input_iterator_tag {};

    struct bidirectional_iterator_tag : public forward_iterator_tag {};

    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    // 迭代器模板
    template<class Category, class T, class Distance =ptrdiff_t, class Pointer=T*, class Reference=T &>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Distance different_type;
    };

    // 以下函数和类用来辅助萃取迭代器中的属性

    /* 用于检测class T中是否有iterator_category的定义
     * 第一个test接受任何参数
     * 第二个test只接受iterator_category*类型的参数
     * 如何T有定义iterator_category，那么会走第二个test，这样value的值会为true；反之走第一个test，value值为false
     */
    template<class T>
    struct has_iterator_cat {
    private:
        struct two {
            char a;
            char b;
        };

        template<class U>
        static two test(...);

        template<class U>
        static char test(typename U::iterator_category* = 0);

    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template<class Iterator, bool>
    struct iterator_traits_impl {};

    template<class Iterator>
    struct iterator_traits_impl<Iterator, true> {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::different_type different_type;
    };

    template<class Iterator, bool>
    struct iterator_traits_helper {};

    template<class Iterator>
    struct iterator_traits_helper<Iterator, true> : public iterator_traits_impl<Iterator,
                std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
                std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value> {};

    template<class Iterator>
    struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

    template<class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        // typedef long int ptrdiff_t
        typedef ptrdiff_t difference_type;
    };

    // 萃取迭代器
    template<class T, class U, bool = has_iterator_cat<iterator_traits<T> >::value>
    // std::is_convertible 用于检查一种类型是否能转换成另一种类型
    struct has_iterator_cat_of :
            public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value> {};

    template<class T, class U>
    struct has_iterator_cat_of<T, U, false> : public m_false_type {};

    template<class Iter>
    struct is_exactly_input_iterator : public m_bool_constant<
                has_iterator_cat_of<Iter, input_iterator_tag>::value &&
                !has_iterator_cat_of<Iter, forward_iterator_tag>::value> {};

    template<class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

    template<class Iter>
    struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

    template<class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

    template<class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

    template<class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

    template<class Iterator>
    struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value ||
                                                is_output_iterator<Iterator>::value> {};

    /* 萃取迭代器的category
     * 继承关系：iterator_traits->iterator_traits_helper->iterator_traits_impl
     */
    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    // 萃取迭代器的difference_type
    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type difference_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 萃取迭代器的value_type
    template<class Iterator>
    typename iterator_traits<Iterator>::value_type value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 计算迭代器之间距离的函数

    // 顺序访问的迭代器的距离计算函数
    template<class InputIterator>

    typename iterator_traits<InputIterator>::difference_type distance_dispatch(
        InputIterator first,
        InputIterator last,
        input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last) {
            ++first;
            ++n;
        }

        return n;
    }

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
        return distance_dispatch(first, last, iterator_category(first));
    }

    // 随机访问迭代器的距离计算函数
    template<class RandomIterator>
    typename iterator_traits<RandomIterator>::difference_type distance_dispatch(
        RandomIterator first,
        RandomIterator last,
        random_access_iterator_tag) {
        return last - first;
    }

    // 让迭代器前进n个距离的函数

    // 顺序访问迭代器的前进
    template<class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
        while(n--) {
            ++i;
        }
    }

    template<class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n) {
        advance_dispatch(i, n, iterator_category(i));
    }

    // 双向迭代器的前进
    template<class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
        if(n >= 0)
            while(n--) ++i;
        else
            while(n++) --i;
    }

    // 随机访问迭代器的前进
    template<class RandomIterator, class Distance>
    void advance_dispatch(RandomIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    }


    /*******************************************************************************************************/
    // 反向迭代器

    template<class Iterator>
    class reverse_iterator {
    private:
        Iterator current;

    public:
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;

        typedef Iterator iterator_type;
        typedef reverse_iterator<Iterator> self;

        // 构造函数
    public:
        reverse_iterator() {}

        explicit reverse_iterator(iterator_type i): current(i) {}

        reverse_iterator(const self& rhs): current(rhs.current) {}

    public:
        iterator_type base() const {
            return this->current;
        }

        reference operator*() const {
            auto tmp = this->current;
            return *(--tmp);
        }

        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            --this->current;
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            --this->current;
            return tmp;
        }

        self& operator--() {
            ++this->current;
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            ++this->current;
            return tmp;
        }

        self& operator+=(difference_type n) {
            this->current -= n;
            return *this;
        }

        self operator+(difference_type n) const {
            return self(this->current - n);
        }

        self& operator-=(difference_type n) {
            this->current += n;
            return *this;
        }

        self operator-(difference_type n) const {
            return self(this->current + n);
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }
    };

    template<class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return rhs.base() - lhs.base();
    }

    template<class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return rhs.base() == lhs.base();
    }

    template<class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return rhs.base() < lhs.base();
    }

    template<class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return rhs - lhs;
    }

    template<class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return !(rhs == lhs.base);
    }

    template<class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return !(rhs < lhs);
    }

    template<class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return !(lhs < rhs);
    }
}
