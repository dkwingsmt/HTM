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







void SpatialPoolerT::spLearn(const data_t *input_data,size_t data_size)
{
    assert(data_size == _PatternList->getPatternSize());
    bool neverAppeared = true;
    for (int i=0;i<_PatternList->size();i++)
    {
        int dis = computeDistance(input_data,_PatternList->getPattern(i),data_size,_PatternList->getPatternSize());
        std::cerr << "[" << dis << "]" ;
       if (dis<=_MaxDist)
       {
           neverAppeared = false;
      //     addTimeLine(preInputID,i);
           break;
       }
    }
    if (neverAppeared)
    {
        _PatternList->push(input_data,data_size); //    addTimeLine(preInputID,_PatternList.getPatternSize()-1);  
    } 
}
const data_t *SpatialPoolerT::spInference(const data_t *input_data,size_t data_size)
{ 
    int minID = 0; 
    int minDist = INFINITE; 
    for (int i=0;i<_PatternList->size();i++)
    { 
        int dist = computeDistance(input_data,_PatternList->getPattern(i),data_size,_PatternList->getPatternSize()); 
        if (dist<minDist) 
        { 
            minID = i;
            minDist = dist;
        }
    }
    data_t * temp = new data_t[_PatternList->size()];
    for (int i=0;i<_PatternList->size();i++)
    {
        temp[i] = 0;
    }
    temp[minID] = 1;
    return temp;
}


}   // namespace htm07
