#include <iostream>
#include "mabustddef.h"

int test_throw() {
    //THROW_LENGTH_ERROR_IF(true,"length_error");
    //THROW_OUT_OF_LENGTH_IF(true,"out_of_length");
    MABUSTL_DEBUG(false);
    THROW_RUNTIME_ERROR_IF(false, "run_time_error1");
    THROW_RUNTIME_ERROR_IF(true, "run_time_error2");
    return 0;
}

int main() {
    //test_throw();
    return 0;
}
