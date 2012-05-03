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
class IntrospectionT;

class LayerT
{
    friend class NodeT;
    friend class IntrospectionT;
public:
    //LayerT(const VecT *input_size, const VecT *node_size); 
    LayerT(const data_t *input, const SpaceT &node_space, 
           const AllocInfoT *input_alloc_info);
    ~LayerT();

    // Use input data to get each node in this layer trained or to infer
    void expose(const data_t *input_data);

    bool fullyReady() const             { return _NumNodeReady >= _NumNode;  }
    size_t dims() const                 { return _Dims;     }
    size_t numNode() const              { return _NumNode;  }
    const SpaceT *nodesSpace() const    { return _NodesSpace;   }

    void forceConclude();

    // Prev members

private:
    //   Called only by NodeT. 
    //   NodeT plz make sure only call it once, for LayerT don't remember
    // which has called it so far, but only the number it is called instead.
    void nodeReadyToConclude(id_t target_id)     { ++_NumNodeReady;    }

    //   This two function used in conclusion process in expose()
    // decided only by this->_NodesSpace
    size_t _nextLayerNodeNum() const;
    id_t _mapNodeToNextLayerNode(id_t src) const;


    void _conclude();

    data_t *_NodeTransferArray;
    data_t *_NodeOutputArray;
    size_t _NextLayerNodeNum;
    AllocInfoT *_NextLayerAllocTable;

    size_t _Dims;
    size_t _NumNodeReady;
    size_t _NumNode; 
    NodeT **_Nodes;
    SpaceT *_NodesSpace;    // Space for the amount of nodes

private:
    // Forbid
    LayerT(const LayerT&);
    const LayerT &operator = (const LayerT&);
};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_LAYER_H__INC  ----- */
