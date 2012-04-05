/*
 * =====================================================================================
 *
 *       Filename:  SpatialPooler.h
 *
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef  _HTM07_SPATIAL_POOLER_H__INC
#define  _HTM07_SPATIAL_POOLER_H__INC
#include "Common.h"
#include <vector>
#include <cassert>
#include "Node.h"
namespace htm07 {


typedef std::vector <int> OneDint_T;



class SpPatternListT
{
public:
    SpPatternListT(size_t PatternSize):_PatternSize(PatternSize){};
    SpPatternListT();
    ~SpPatternListT();
    std::vector <data_t*> _Patterns;
    inline size_t size(){return _Patterns.size();};
    size_t _PatternSize;
    inline size_t getPatternSize(){return _PatternSize;};
    inline void setPatternSize(size_t PatternSize){_PatternSize=PatternSize;};
    inline const data_t* getPattern(int i){return _Patterns[i];};
    void push(const data_t * input,size_t data_size);
};

class SpatialPoolerT
{
public:

    int preInputID;
    data_t _MaxDist;
    SpPatternListT* _PatternList;
    size_t _PatternSize;
    std::vector <OneDint_T> _TemporalTable;
    bool _learned; 
    size_t getQuantsSize() {return _PatternList->size();};
   // void addTimeLine(int prevID,int currID);
    void spLearn(const data_t *input_data,size_t data_size);//main process
    const data_t * spInference(const data_t *input_data,size_t data_size); 
    bool learned() {return _learned;};

    SpatialPoolerT(NodeT *node){};
    SpatialPoolerT(size_t PatternSize){_PatternList->setPatternSize(PatternSize);}
    ~SpatialPoolerT()   {};

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPATIAL_POOLER_H__INC  ----- */
