#pragma once

/*
 * time: 2025-1-20
 * author: mabu
 */

#include <stdexcept>
#include <cassert>


namespace mabustl {
#define MABUSTL_DEBUG(expr) assert(expr)

#define THROW_LENGTH_ERROR_IF(expr,what) \
    if((expr)) throw std::length_error(what)

#define THROW_OUT_OF_LENGTH_IF(expr,what) \
    if((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr,what) \
    if((expr)) throw std::runtime_error(what)
}