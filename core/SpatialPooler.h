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
#include "common.h"
#include <vector>

namespace htm07 {

class SpPatternListT;

class SpatialPoolerT
{
public:
    SpatialPoolerT(NodeT *node)    {}
    ~SpatialPoolerT()   {}

    void spExpose(data_t *input_data);//main process
    bool learned() const;

    SpPatternListT* patternList();
    size_t patternSize() const;

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPATIAL_POOLER_H__INC  ----- */
