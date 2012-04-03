/*
 * =====================================================================================
 *
 *       Filename:  Common.cpp
 *
 *    Description:  
 *
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "Common.h"

namespace htm07 {

void initializeVec(VecT *vec, size_t dims)
{
    vec->dims = dims;
    vec->max = new size_t[dims];
    assert(vec->max && "Allocation failed.");
}

}   // namespace htm07
