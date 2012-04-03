/*
 * =====================================================================================
 *
 *       Filename:  Node.h
 *
 *    Description:  
 *
 * =====================================================================================
 */


#ifndef  _HTM07_NODE_H__INC
#define  _HTM07_NODE_H__INC

#include <cassert>
#include "Common.h"
#include "Layer.h"

namespace htm07 {

class SpatialPoolerT;
class SubSpaceT;

class NodeT
{
public:
    NodeT(LayerT *layer, id_t node_id, const VecT *input_std_size);
    ~NodeT();

    void nodeExpose(const data_t *input);
    
    bool learned() const                { return _Learned;  }

private:
    data_t *_outputData()               { return _Layer->_Output;   }
    const data_t *_outputData() const   { return _Layer->_Output;   }

    LayerT *_Layer;     // The layer it belongs
    id_t _Id;           // The id that it is in its layer
    bool _Learned;      
    SubSpaceT *_InputSpace;     // The subspace of input from the layer
    SubSpaceT *_OutputSpace;    // The subspace of output to the layer

    SpatialPoolerT *_Sp;
};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_NODE_H__INC  ----- */
