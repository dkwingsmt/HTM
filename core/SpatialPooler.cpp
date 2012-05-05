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

SpPatternListT::~SpPatternListT()
{
        for (int i=0;i<_Patterns.size();i++)
        {
            if (_Patterns[i]!=NULL)
              delete _Patterns[i];
        }
        _Patterns.clear();
}

void SpPatternListT::push(const data_t *input,size_t data_size)
{
    assert(data_size==_PatternSize);
    data_t * temp = new data_t[data_size];
    for (int i=0;i<data_size;i++)
    {
        temp[i] = input[i];
    }
    _Patterns.push_back(temp);

}


void SpatialPoolerT::spLearn()
{
    bool neverAppeared = true;
    for (int i=0;i<_PatternList->size();i++)
    {
        int dis = computeDistance(_InputData,_PatternList->getPattern(i),_PatternSize);
       if (dis<=_MaxDist)
       {
           neverAppeared = false;
      //     addTimeLine(preInputID,i);
           break;
       }
    }
    if (neverAppeared)
    {
        _PatternList->push(_InputData,_PatternSize); //    addTimeLine(preInputID,_PatternList.getPatternSize()-1);  
    } 
}
void SpatialPoolerT::spInference()
{ 
    int minID = 0; 
    int minDist = INFINITE; 
    for (int i=0;i<_PatternList->size();i++)
    { 
        int dist = computeDistance(_InputData,_PatternList->getPattern(i),_PatternSize); 
        if (dist<minDist) 
        { 
            minID = i;
            minDist = dist;
        }
    }
    for (int i=0;i<_PatternList->size();i++)
    {
        _OutputData[i] = 0;
    }
    _OutputData[minID] = 1;
}

void SpatialPoolerT::setOutputDest(const AllocInfoT &alloc_info)
{
   _OutputData = alloc_info.pos;
}
}   // namespace htm07
