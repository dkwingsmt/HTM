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
#include "Node.h"

namespace htm07 {

LayerT::LayerT(const VecT *input_size, const VecT *node_size) :
    _NumNodeLearned(0)
{
    // Use input allocation table to initialize NodeTs


    assert(input_size->dims == node_size->dims);
    _Dims = input_size->dims;
    _InputSpace = new SpaceT(input_size);
    assert(_InputSpace && "Allocation failed.");

    VecT node_space_max;
    initializeVec(&node_space_max, dims());
    for(size_t i = 0; i < _Dims; ++i)
    {
        node_space_max.max[i] = (size_t)std::ceil(
                    (float)(input_size->max[i]) / (float)(node_size->max[i]));
    }

    _NodesSpace = new SpaceT(&node_space_max);
    assert(_NodesSpace && "Allocation failed.");

    /*
    // TODO: How to judge the size of output? Now I'm giving a constant size.
    VecT output_space_max;
    initializeVec(&output_space_max, dims());
    for(size_t i = 0; i < _Dims; ++i)
    {
        output_space_max.max[i] = 4 * node_space_max.max[i];
    }
    _OutputSpace = new SpaceT(&output_space_max);
    assert(_OutputSpace && "Allocation failed.");
    _Output = new data_t[_OutputSpace->getTotalSize()];
    assert(_Output && "Allocation failed.");

    _NumNode = _NodesSpace->getTotalSize();
    _Nodes = new NodeT *[numNode()];
    assert(_Nodes && "Allocation failed.");
    for(size_t i = 0; i < numNode(); ++i)
    {
        _Nodes[i] = new NodeT(this, i, node_size);
    }
    */

    delete[] node_space_max.max;
    delete[] output_space_max.max;
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
    delete _OutputSpace;
}

void LayerT::expose(const data_t *input_data)
{
    bool before_learned = fullyLearned();
    for(size_t i = 0; i < numNode(); ++i)
    {
        _Nodes[i]->nodeExpose(input_data);
    }

    //   Such condition never happens that this layer is learned before 
    // but turned unlearned after.
    assert(!(before_learned && !fullyLearned()));

    // if turned learned this round
    if(!before_learned && fullyLearned())
    {
        // all Node::buildTpStepOne()
        //      Sp::sortGroup
        //      new Tp(Table)
        //
        // new data_t *(sum_center_num) sp_output
        // generate sp_output allocation table
        // new data_t *(sum_group_num)  tp_output (or next_layer_input)
        // generate tp_output allocation table
        // generate next_layer_input allocation table
        //
        // all Node::buildTpStepTwo()
    }

}

}   // namespace htm07
