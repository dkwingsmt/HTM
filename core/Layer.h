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
    LayerT(const SpaceT &node_space, const AllocInfoT *input_alloc_info);
    ~LayerT();

    // Use input data to get each node in this layer trained or to infer
    void expose();


    bool fullyReady() const             { return _NumNodeReady >= _NumNode;  }
    size_t dims() const                 { return _Dims;     }
    size_t numNode() const              { return _NumNode;  }
    const SpaceT *nodesSpace() const    { return _NodesSpace;   }
    const NodeT *node(id_t id) const    { return _Nodes[id];    }

    SpaceT ** getNextLayerSpaceT() const;
    void forceConclude()                {_NumNodeReady = _NumNode; _conclude();};
    AllocInfoT * getOutputAllocInfo() const  { return _NextLayerAllocTable;};

    // Prev members

private:
    //   Called only by NodeT. 
    //   NodeT plz make sure only call it once, for LayerT don't remember
    // which has called it so far, but only the number it is called instead.
    void nodeReadyToConclude(id_t target_id)     { ++_NumNodeReady;    }

    //   This two function used in conclusion process in expose()
    // determined only by this->_NodesSpace
    // TODO(mt): generalize them two
    void _nextLayerNodesSpace(SpaceT **out) const;
    id_t _mapNodeToNextLayerNode(id_t src) const;
    void _nextLayerNodesSpace(SpaceT **out,size_t numbertodivide) const;
    id_t _mapNodeToNextLayerNode(id_t src,size_t numbertodivide) const;
    void _conclude();

    size_t _Dims;
    size_t _NumNodeReady;
    size_t _NumNode; 
    NodeT **_Nodes;
    SpaceT *_NodesSpace;    // Space for the amount of nodes

    data_t *_NodeOutputArray;
    data_t *_NodeTransferArray;
    SpaceT *_NextLayerNodesSpace;
    AllocInfoT *_NextLayerAllocTable;
    SpaceT * _NextLayerSpaceSave;

private:
    // Forbid
    LayerT(const LayerT&);
    const LayerT &operator = (const LayerT&);
};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_LAYER_H__INC  ----- */
