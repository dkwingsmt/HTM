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
#include <vector>
#include <queue>
#include <cassert>
#include "Common.h"

namespace htm07 {

    
void formTemperalGroup(const float* adj_mat, size_t size, size_t ** group_info,size_t * group_num); 

class IntrospectionT;

typedef std::vector <float> OneDfloat_T;

class SpPatternListT // store the patterns, or the so-called "quantization centers"
{
    friend class IntrospectionT;
public:
    SpPatternListT(size_t PatternSize):_PatternSize(PatternSize){};
    ~SpPatternListT();
    size_t size(){return _Patterns.size();}; // returns how many patterns are stored
    size_t getPatternSize(){return _PatternSize;}; // returns the size of each pattern
    void setPatternSize(size_t PatternSize){_PatternSize=PatternSize;}; 
    const data_t* getPattern(int i){return _Patterns[i];}; // get pattern by id
    void push(const data_t * input,size_t data_size); 
    // input pattern, will new a data_t * to fully copy the data,deleted in the ~Sp...
private:
    size_t _PatternSize;
    std::vector <data_t*> _Patterns;
};

class SpatialPoolerT
{
    friend class IntrospectionT;
private:
    
    data_t* _InputData;
    data_t* _OutputData;
    int _PreInputID;// not be used yet ,for the tp
    data_t _MaxDist;// the maxdist to distinguish differente patterns.
    SpPatternListT* _PatternList;
    size_t _PatternSize;// the size of a single pattern.
    bool _Learned; 
    bool _Concluded;
    std::vector <OneDfloat_T> _AdjMat;
public:    
    size_t GetCentersNum() const {return _PatternList->size();}; // return how many patterns have been stored
    bool readyToConclude() const {return _Learned;};
    //   Divide centers into groups, output groups info(size_t[] indicating 
    // which group each center belongs to, and the total group amount.
    //   Do not change self or any storage jobs, only compute and output.
    void sortGroup(size_t **center_group_info, size_t *group_num) const;
    void setConcluded(){_Concluded = true;};      // Will delete temp members (like the adjacency matrix)
    void setOutputDest(const AllocInfoT &alloc_info);

    void addTimeLine(size_t prevID, size_t currID);
    void spLearn();//main process of learning
    void  spInference(); 
    bool concluded ()const {return _Concluded;}; // returns if sp has finished learning process.

    // at spLearn and spInference
    SpatialPoolerT(data_t* input_data,size_t PatternSize){
        _InputData = input_data;
        _PatternList = new SpPatternListT(PatternSize);
        assert(_PatternList && "Allocation failed.");
        _Learned = false;
        _Concluded = false;
        _PreInputID = -1;
    }
    ~SpatialPoolerT()   {};

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPATIAL_POOLER_H__INC  ----- */
