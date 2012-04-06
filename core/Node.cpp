/*
 * =====================================================================================
 *
 *       Filename:  Node.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "Node.h"
#include "SpatialPooler.h"

namespace htm07 {

template < typename T >
    inline T __minBinary(const T &lhs, const T &rhs)
    {
        return (lhs < rhs) ? lhs : rhs;
    }

NodeT::NodeT(LayerT *layer, id_t node_id, const VecT *input_std_size) :
    _Learned(false)
{
    _Layer = layer;
    _Id = node_id;
    size_t dims = _Layer->dims();
    assert(dims == input_std_size->dims);

    VecT input_start_pos;
    initializeVec(&input_start_pos, dims);
    VecT input_node_size;
    initializeVec(&input_node_size, dims);
    VecT output_start_pos;
    initializeVec(&output_start_pos, dims);
    VecT output_node_size;
    initializeVec(&output_node_size, dims);

    SpaceT *nodes_space = _Layer->_NodesSpace;
    SpaceT *input_space = _Layer->_InputSpace;
    SpaceT *output_space = _Layer->_OutputSpace;
    for(size_t i = 0; i < dims; ++i)
    {
        input_start_pos.max[i] = nodes_space->getCoord(_Id, i) 
                                    * input_std_size->max[i];
        input_node_size.max[i] = __minBinary<size_t>(
                    input_std_size->max[i],
                    input_space->getMaxCoord(i) - input_start_pos.max[i]);
        assert(input_node_size.max[i] > 0);

        // TODO(mt): How to determine the output size? (See Layer::Layer())
        output_start_pos.max[i] = nodes_space->getCoord(_Id, i) * 4;
        output_node_size.max[i] = 4;
    }
    _Layer->_InputSpace->getSubSpace(&input_start_pos, &input_node_size, 
                                     &_InputSpace);
    _Layer->_InputSpace->getSubSpace(&output_start_pos, &output_node_size, 
                                     &_OutputSpace );

    _Sp = new SpatialPoolerT(this);
    assert(_Sp && "Allocation failed.");

    delete[] input_start_pos.max;
    delete[] input_node_size.max;
    delete[] output_start_pos.max;
    delete[] output_node_size.max;
}

void NodeT::nodeExpose(const data_t *input)
{
    _Sp->spExpose(input);
    if(_Sp->learned() && !learned())
    {
        _Layer->nodeSetLearned(_Id);

    }
}

}   // namespace htm07
