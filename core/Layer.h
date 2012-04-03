/*
 * =====================================================================================
 *
 *       Filename:  Layer.h
 *
 *    Description:  
 *
 * =====================================================================================
 */


#ifndef  _HTM07_LAYER_H__INC
#define  _HTM07_LAYER_H__INC

#include "Space.h"

namespace htm07 {

class NodeT;

class LayerT
{
    friend class NodeT;
public:
    LayerT(VecT *input_size, VecT *node_size)    {}
    ~LayerT()   {}

    void expose(data_t *input_data, const SubSpace *input_space);

private:
    void nodeSetLearned(id_t target_id);

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_LAYER_H__INC  ----- */
