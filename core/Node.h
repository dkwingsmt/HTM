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

/* Class NodeT
 * 
 * The class that act as a node. A layer contains of an array of node, each of
 *   which is responsible of a small chunk of input, calculate individually, 
 *   and generate output to the given part of the layer.
 * A node contains a SpatialPooler and a TemperalPooler.
 */
class NodeT
{
public:
    /* Constructor
     *   
     *     Besides basic member assignment, also calculated __InputSpace,
     *   __OutputSpace according to input. __Learn is set false.
     *   Will create a sp. (Tp creation is postponed until sp is learned.)
     * Param:
     *   layer
     *      The layer it belongs to.
     *   node_id    
     *      Where the node is placed inside the layer.
     *   input_std_size
     *      The default size that one node manages, used by this node
     *      unless no enough space remains. 
     */
    NodeT(LayerT *layer, id_t node_id, const VecT *input_std_size);

    // Regular deletion.
    ~NodeT();

    /* Function nodeExpose
     *
     *     The main input function. Node receives the input and hand it to
     *   spatial pooler. If sp is already learned, hand the result of sp to tp.
     *   If sp is learned during this exposure, will fetch its result and create
     *   a tp, then inform the layer.
     * Param:
     *   input
     *     The complete input data. The useful region is decided by _InputSpace.
     *     which is calculated in the constructor.
     */
    void nodeExpose(const data_t *input);
    
    //   Whether this node completed learning. Equivalent to the learned state
    // of its sp. Returns true if completed.
    bool learned() const                { return _Learned;  }

private:
    data_t *_outputData()               { return _Layer->_Output;   }
    const data_t *_outputData() const   { return _Layer->_Output;   }

    LayerT *_Layer;     // The layer it belongs
    id_t _Id;           // The id that it is in its layer
    bool _Learned;      // State flag whether node completed learning
    SpaceT *_InputSpace;    // The subspace of input from the layer
    SpaceT *_OutputSpace;   // The subspace of output to the layer

    SpatialPoolerT *_Sp;

    data_t *_InputData;
};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_NODE_H__INC  ----- */
