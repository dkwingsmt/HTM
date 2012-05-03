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
#include <cassert>
#include "Node.h"
#include "Common.h"

namespace htm07 {

class IntrospectionT;

typedef std::vector <int> OneDint_T;

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

    int _PreInputID;// not be used yet ,for the tp
    data_t _MaxDist;// the maxdist to distinguish differente patterns.
    SpPatternListT* _PatternList;
    size_t _PatternSize;// the size of a single pattern.
    std::vector <OneDint_T> _TemporalTable; // used for tp
    bool _learned; 
public:    
    // TODO(mt):Change to "GetCentersNum const"
    size_t getQuantsSize() {return _PatternList->size();}; // return how many patterns have been stored
    bool readyToConclude() const;
    //   Divide centers into groups, output groups info(size_t[] indicating 
    // which group each center belongs to, and the total group amount.
    //   Do not change self or any storage jobs, only compute and output.
    void sortGroup(size_t **center_group_info, size_t *group_num) const;
    void setConcluded();      // Will delete temp members (like the adjacency matrix)
    void setOutputDest(const AllocInfoT &alloc_info);

    // void addTimeLine(int prevID,int currID);
    // TODO(mt):Complete, and use internal input_data instead of parameters
    void spLearn(const data_t *input_data,size_t data_size);//main process of learning
    // TODO(mt):Output to the output position that is set in setOutputDest
    //          return void
    // inference process , will new a data_t as output, need to delete it out side
    // TODO(mt):Change parameter to none and return to void.
    const data_t * spInference(const data_t *input_data,size_t data_size); 
    // TODO(mt):Change to "concluded const"
    bool learned() {return _learned;}; // returns if sp has finished learning process.

    // TODO(mt): receive an extra data_t * as input_data instead of parameter
    // at spLearn and spInference
    SpatialPoolerT(size_t PatternSize){
        _PatternList = new SpPatternListT(PatternSize);
        assert(_PatternList && "Allocation failed.");
    }
    ~SpatialPoolerT()   {};

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPATIAL_POOLER_H__INC  ----- */
