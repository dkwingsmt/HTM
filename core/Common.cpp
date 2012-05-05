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
data_t computeDistance(const data_t* input1, const data_t* input2, size_t size)
{
    data_t sum=0;
    for (int i=0;i<size;i++)
    {
        sum += (input1[i] - input2[i])*(input1[i]-input2[i]);
    }
    return sum;
}

void initializeVec(VecT *vec, size_t dims)
{
    vec->dims = dims;
    vec->max = new size_t[dims];
    assert(vec->max && "Allocation failed.");
}

}   // namespace htm07
