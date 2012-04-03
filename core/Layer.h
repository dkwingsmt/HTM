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
    LayerT(VecT *input_size, VecT *node_size); 
    ~LayerT();

    void expose(data_t *input_data, const SubSpace *input_space);

    bool fullyLearned() const           { return _NumNodeLearned == _NumNode;  }
    size_t dims() const                 { return _Dims;     }
    size_T numNode() const              { return _NumNode;  }
    const data_t *output() const        { return _Output;   }
    const SpaceT *outputSpace() const   { return _OutputSpace;  }
    const SpaceT *inputSpace() const    { return _InputSpace;   }
private:
    void nodeSetLearned(id_t target_id)     { ++_NumNodeLearned;    }

    size_t _Dims;
    size_t _NumNodeLearned;
    size_t _NumNode; 

    NodeT *_Nodes;
    
    SpaceT *_InputSpace;    // Space for the size of input
    SpaceT *_NodesSpace;    // Space for the amount of nodes
    SpaceT *_NodeStdSizeSpace;  // Space for the standard size of a node 
    data_t *_Output;
    SpaceT *_OutputSpace;   // Space that indicate the size of output

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_LAYER_H__INC  ----- */
