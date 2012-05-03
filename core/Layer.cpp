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
    delete[] output_space_max.max;
    */

    delete[] node_space_max.max;
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
    const size_t node_num = numNode();
    for(size_t i = 0; i < node_num; ++i)
    {
        _Nodes[i]->nodeExpose(input_data);
    }

    //   Such condition never happens that 
    //   this layer is learned before but turned unlearned after.
    assert(!(before_learned && !fullyLearned()));

    // if turned learned right at this round
    if(!before_learned && fullyLearned())
    {
        _conclude();
    }
}

void LayerT::_conclude()
{
    // Abbreviation and terms:
    //   node:      nodes of this layer
    //   nlnode:    nodes of the next layer
    //   transfer:  the data_t[] provided for sp as output and for tp as input
    //   output:    the data_t[] provided for tp in this-layer nodes as output
    //              and next-layer nodes as input
    //
    // This function generate allocation tables:
    // [Raw]
    //   An array of size_t, 
    // each indicates the size of one node from the next layer
    //   An array of {next_node_id, pos, len}s, 
    // each corresponds to one node from this layer
    //   An array of size_t for the transfer data from sp to tp
    // [Mature]
    //   An array of AllocInfoT for nodes of the next layer
    //   An array of AllocInfoT for nodes of this layer as output 
    //   An array of AllocInfoT for nodes of this layer as transfer
    //      (The last two will only be used for NodeT::concludeStepTwo() 
    //         so probably they won't really be created and stored.)
    //
    // Structure:
    //  Assume,
    //  this layer nodes             next layer nodes
    //    |--|--|--|--|                |-----|-----|
    //    |0 |1 |2 |3 |                |     |     |
    //    |--|--|--|--|                |  0  |  1  |
    //    |4 |5 |6 |7 |                |     |     |
    //    |--|--|--|--|                |-----|-----|
    //         ...                          ...
    //  Then the output allocation tables are like
    //  nlnode_input[0]     nlnode_input[1]     nlnode_input[2]
    //  |                   |                   |
    //  v                   v                   v
    //  |-------------------|-------------------|--          --| 
    //  |    |    |    |    |    |    |    |    |     ...      |
    //  |-------------------|-------------------|--          --|
    //  ^    ^    ^    ^    ^
    //  |    |    |    node_output[5]           
    //  |    |    node_output[4]
    //  |    node_output[1] |                       
    //  node_output[0]      node_output[2]
    //  |____| <- node[0].groups_num
    //
    //  The transfer allocation tables are like
    //  node_transfer[0]   node_transfer[2]
    //  |         node_transfer[1]  node_transfer[3]                 
    //  v         v        v        v 
    //  |---------|--------|--------|--------|--          --| 
    //  |         |        |        |        |     ...      |
    //  |---------|--------|--------|--------|--          --|
    //  |_________| <- node[0].centers_num
    
    const size_t node_num = numNode();

    // === Generate raw allocation table ===
    
    struct _AllocRawInfoT {
        id_t nlnode_id; size_t pos; size_t len;
    };
    const size_t nlnode_num = _nextLayerNodeNum();
    size_t sum_input_size = 0;      // also is sum_groups_num
    // sum of sizes of the nodes that belongs to each nlnode
    size_t *nlnode_input_sizes = new size_t[nlnode_num];
    assert(nlnode_input_sizes && "Allocation failed.");
    for(size_t i = 0; i < nlnode_num; ++i)
        nlnode_input_sizes[i] = 0;

    size_t sum_centers_num = 0;
    _AllocRawInfoT *output_raw_info = new _AllocRawInfoT[node_num];
    size_t *node_centers_nums = new size_t[node_num];
    assert(output_raw_info && "Allocation failed.");
    assert(node_centers_nums && "Allocation failed.");

    // Below used as temp vars
    _AllocRawInfoT *this_raw_info;   
    NodeT *this_node;

    for(size_t node_id = 0; node_id < node_num; ++node_id)
    {
        this_node = _Nodes[node_id];
        this_raw_info = output_raw_info + node_id;

        assert(this_node->readyToConclude());
        this_node->concludeStepOne();

        node_centers_nums[node_id] = this_node->centersNum();
        sum_centers_num += node_centers_nums[node_id];
        this_raw_info->nlnode_id = _mapNodeToNextLayerNode(node_id);
        this_raw_info->pos = nlnode_input_sizes[this_raw_info->nlnode_id];
        this_raw_info->len = this_node->groupsNum();
        nlnode_input_sizes[this_raw_info->nlnode_id] += this_raw_info->len;
        sum_input_size += this_raw_info->len;
    }

    // === Generate mature allocation table ===
    
    if(_NodeTransferArray)
        delete[] _NodeTransferArray;
    if(_NodeOutputArray)
        delete[] _NodeOutputArray;
    _NodeTransferArray = new data_t[sum_centers_num];
    _NodeOutputArray = new data_t[sum_input_size]; 
    AllocInfoT *_NextLInputAlloc = new AllocInfoT[nlnode_num];
    AllocInfoT transfer_alloc;
    AllocInfoT output_alloc;
    // TODO(mt): to continue here
    assert(_NodeTransferArray && _NodeOutputArray 
           && _NextLInputAlloc && "Allocation failed.");

    data_t *now_data_tail;   // Temp var 
    id_t target_nlnode;      // Temp var, the nlnode that this node belongs
    now_data_tail = _NodeOutputArray;
    for(size_t nlnode_id = 0; nlnode_id < nlnode_num; ++nlnode_id)
    {
        _NextLInputAlloc[nlnode_id].len = nlnode_input_sizes[nlnode_id];     
        _NextLInputAlloc[nlnode_id].pos = now_data_tail;
        now_data_tail += nlnode_input_sizes[nlnode_id];
    }
    now_data_tail = _NodeTransferArray;
    for(size_t node_id = 0; node_id < node_num; ++node_id)
    {
        transfer_alloc.pos = now_data_tail;
        transfer_alloc.len = node_centers_nums[node_id];
        now_data_tail += node_centers_nums[node_id];

        target_nlnode = output_raw_info[node_id].nlnode_id;
        output_alloc.pos = _NextLInputAlloc[target_nlnode].pos
                                    + output_raw_info[node_id].pos;
        output_alloc.len = output_raw_info[node_id].len;

        _Nodes[node_id]->concludeStepTwo(transfer_alloc, output_alloc);
    }

    // === Garbage collection ===
    delete[] nlnode_input_sizes;
    delete[] output_raw_info;
    delete[] node_centers_nums;
}

}   // namespace htm07
