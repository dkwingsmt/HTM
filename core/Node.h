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
#include "SpatialPooler.h"
#include "TemporalPooler.h"

#include <iostream>

namespace htm07 {

class SpatialPoolerT;
class SubSpaceT;
class IntrospectionT;

/* Class NodeT
 * 
 * The class that act as a node. A layer contains of an array of node, each of
 *   which is responsible of a small chunk of input, calculate individually, 
 *   and generate output to the given part of the layer.
 * A node contains a SpatialPooler and a TemperalPooler.
 */
class NodeT
{
    friend class IntrospectionT;
public:
    /* Constructor
     *   
     *     Besides basic member assignment
     *     Will create a sp. (Tp creation is postponed until sp is learned.)
     * Param:
     *   layer
     *      The layer it belongs to.
     *   node_id    
     *      Where the node is placed inside the layer.
     *   alloc_info
     *      Where the input is placed.
     */
    NodeT(LayerT *layer, id_t node_id, const AllocInfoT &alloc_info);

    // Regular deletion.
    ~NodeT();

    /* Function nodeExpose
     *
     *     The main input function. Node receives the input and hand it to
     *   spatial pooler. If sp is already learned, hand the result of sp to tp.
     *   If sp is ready to conclude during this exposure, will inform the layer.
     *   When all nodes are ready, layer will ask all nodes to conclude together.
     */
    void nodeExpose();
    
    bool readyToConclude() const            { return _Sp->readyToConclude();  }
    bool concluded() const                  { return _Concluded;  }

    void concludeStepOne();
    void concludeStepTwo(data_t *sp_output_pos,
                         data_t *tp_output_pos);
    size_t centersNum() const           { return _Sp->getCentersNum();  }
    size_t groupsNum() const;

private:
    data_t *_outputData()               { return _Layer->_NodeOutputArray;   }
    const data_t *_outputData() const   { return _Layer->_NodeOutputArray;   }

    LayerT *_Layer;     // The layer it belongs
    id_t _Id;           // The id that it is in its layer
    bool _Concluded;    // State flag whether node completed learning

    SpatialPoolerT *_Sp;
    TemporalPoolerT *_Tp;
    const data_t *_InputData;
    size_t _InputLen;
};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_NODE_H__INC  ----- */
