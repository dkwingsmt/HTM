/*
 * =====================================================================================
 *
 *       Filename:  SpatialPooler.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "SpatialPooler.h"

namespace htm07 {

void SpatialPooler::spLearn(data_t *input_data,size_t data_size)
{
    assert(data_size == _PatternList.getPatternSize());
    bool neverAppeared = true;
    for (int i=0;i<_PatternList.size();i++)
    {
       if (computeDistance(input_data,_PatternList.getPattern(i),data_size,_PatternList.getPatternSize())<=_MaxDist)
       {
           neverAppeared = false;
      //     addTimeLine(preInputID,i);
           break;
       }
    }
    if (neverAppeared)
    {
        _PatternList.push(input_data);
    //    addTimeLine(preInputID,_PatternList.getPatternSize()-1);   
    }
}

data_t *spInference(data_t *input_data,size_t data_size)
{
    int minID = 0;
    int minDist = INFINITE;
    for (int i=0;i<_PatternList.size();i++)
    {
        int dist = computeDistance(input_data,_PatternList.getPattern(i),data_size,_PatternList.getPatternSize());
        if (dist<minDist)
        {
            minID = i;
            minDist = dist;
        }
    }
    
}


}   // namespace htm07
