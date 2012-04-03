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
data_t computeDistance(data_t* input1,data_t* input2,size_t size1,size_t size2)
{
    if (size1!=size2)
      return INFINITE;
    data_t sum=0;
    for (int i=0;i<size1;i++)
    {
        sum += (input1[i] - input2[i])*(input1[i]-input2[i]);
    }
    return sum;
}
}   // namespace htm07
