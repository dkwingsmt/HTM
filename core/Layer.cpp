/*
 * =====================================================================================
 *
 *       Filename:  Layer.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include "Layer.h"

namespace htm07 {

using std::assert;

LayerT::LayerT(VecT *input_size, VecT *node_size) :
    _NumNodeLearned(0)
{
    assert(input_size->dims == node_size->dims);
    _Dims = input_size->dims;
    _InputSpace = new SpaceT(input_size);
    assert(_InputSpace && "Allocation failed.");

    VecT node_space_max;
    node_space_max.dims = _Dims;
    node_space_max.max = new size_t[_Dims];
    assert(node_space_max->max && "Allocation failed.");
    for(size_t i = 0; i < _Dims; ++i)
    {
        node_space_max->max = (size_t)std::ceil(
                    (float)input_size->max[i] / (float)node_size->max[i]);
    }

    _NodesSpace = new SpaceT(&node_space_max);
    assert(_NodesSpace && "Allocation failed.");

    _NodeStdSizeSpace = new SpaceT(&node_size);
    assert(_NodeStdSizeSpace && "Allocation failed.");

    // TODO: How to judge the size of output? Now I'm giving a constant size.
    VecT output_space_max;
    output_space_max.dims = _Dims;
    output_space_max.max = new size_t[_Dims];
    assert(output_space_max->max && "Allocation failed.");
    for(size_t i = 0; i < _Dims; ++i)
    {
        output_space_max.max[i] = 4 * _node_space_max.max[i];
    }
    _OutputSpace = new SpaceT(&output_space_max);
    assert(_OutputSpace && "Allocation failed.");
    _Output = new data_t[_OutputSpace->getSize()];
    assert(_Output && "Allocation failed.");

    size_t _NumNode = _NodesSpace->getSize();
    _Nodes = new (NodeT *)[numNode()];
    assert(_Nodes && "Allocation failed.");
    for(size_t i = 0; i < numNode(); ++i)
    {
        _Nodes[i] = new NodeT(this, i);
    }

    delete[] node_space_max->max;
    delete[] output_space_max->max;
}

LayerT::~LayerT()
{
    for(size_t i = 0; i < numNode(); ++i)
    {
        delete _Nodes[i];
    }
    delete[] _Nodes;
    delete _InputSpace;
    delete _NodesSpace;
    delete _NodeSizeSpace;
    delete _NodeStdSizeSpace;
    delete _OutputSpace;
}

}   // namespace htm07
