/*
 * =====================================================================================
 *
 *       Filename:  TemporalPooler.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "TemporalPooler.h"
#include <queue>

#include <iostream>
#include <vector>

namespace htm07 {

void TemporalPoolerT::TpInference()
{
    assert(_InputData!=NULL);
    assert(_OutputData!=NULL);
    for (size_t i=0;i<_OutputSize;i++)
      _OutputData[i] = 0;
    for (size_t i=0;i<_InputSize;i++)
    {
        if (_InputData[i]==1)
        {
            _OutputData[_Groups[i]]++;
        }
    }
}

}

