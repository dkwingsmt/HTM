/*
 * =====================================================================================
 *
 *       Filename:  TemporalPooler.h
 *
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef  _HTM07_TEMPORAL_POOLER_H__INC
#define  _HTM07_TEMPORAL_POOLER_H__INC
#include <vector>
#include <cassert>
//#include "Node.h"
#include "Common.h"

namespace htm07{


class TemporalPoolerT
{
private :
    size_t* _Groups;
    size_t _GroupsNum;
    size_t _InputSize;
    size_t _OutputSize;
    data_t* _InputData;
    data_t* _OutputData;
public :
    TemporalPoolerT(size_t* groups,size_t groupnum,size_t inputsize)
    :_Groups(groups),_GroupsNum(groupnum),_InputSize(inputsize),_OutputSize(groupnum){}
    ~TemporalPoolerT(){if (_Groups!=NULL) delete []_Groups;}
    void setIODest(data_t* inputdata,data_t* outputdata){
    _InputData = inputdata; _OutputData = outputdata;};
    void TpInference();

};



}
#endif
