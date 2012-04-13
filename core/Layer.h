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
    LayerT(const VecT *input_size, const VecT *node_size); 
    ~LayerT();

    // Use input data to get each node in this layer trained or to infer
    void expose(const data_t *input_data);

    bool fullyLearned() const           { return _NumNodeLearned >= _NumNode;  }
    size_t dims() const                 { return _Dims;     }
    size_t numNode() const              { return _NumNode;  }
    const data_t *output() const        { return _Output;   }
    const SpaceT *outputSpace() const   { return _OutputSpace;  }
    const SpaceT *inputSpace() const    { return _InputSpace;   }
    const SpaceT *nodesSpace() const    { return _NodesSpace;   }

    void p() const;
private:
    //   Called only by NodeT. 
    //   NodeT plz make sure only call it once, for LayerT don't remember
    // which has called it so far, but only the number it is called instead.
    void nodeSetLearned(id_t target_id)     { ++_NumNodeLearned;    }

    size_t _Dims;
    size_t _NumNodeLearned;
    size_t _NumNode; 

    NodeT **_Nodes;
    
    SpaceT *_NodesSpace;    // Space for the amount of nodes

    SpaceT *_InputSpace;    // Space for the size of input
    data_t *_Output;
    SpaceT *_OutputSpace;   // Space that indicate the size of output

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_LAYER_H__INC  ----- */
