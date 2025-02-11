#pragma once

/*
 * time: 2025-1-27
 * author: mabu
 */

// TODO: 未完成堆相关算法

#include "mabu_iterator.h"

namespace mabustl {
    template<class RandomIter, class Distance, class T>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value) {
        auto parent = (holeIndex - 1) / 2;
        while(holeIndex > topIndex && *(first + parent) < value) {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }


}
