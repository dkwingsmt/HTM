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

LayerT::LayerT(const SpaceT &node_space, const AllocInfoT *input_alloc_info) :
    _NumNodeReady(0), _NodeOutputArray(NULL), _NodeTransferArray(NULL),
    _NextLayerNodesSpace(NULL), _NextLayerAllocTable(NULL)
{
    // Use input allocation table to initialize NodeTs

    _Dims = node_space.getDimension();
    _NumNode = node_space.getSize();
    _NodesSpace = new SpaceT(node_space);
    assert(_NodesSpace && "Allocation failed.");

    _Nodes = new NodeT *[numNode()];
    assert(_Nodes && "Allocation failed.");
    for(size_t i = 0; i < numNode(); ++i)
    {
        _Nodes[i] = new NodeT(this, i, input_alloc_info[i]);
        assert(_Nodes[i] && "Allocation failed.");
    }

}

LayerT::~LayerT()
{
    if(_Nodes)
    {
        for(size_t i = 0; i < numNode(); ++i)
        {
            SAFE_DELETE(_Nodes[i]);
        }
        delete[] _Nodes;
    }
    SAFE_DELETE(_NodesSpace);
    SAFE_DELETE(_NodeTransferArray);
    SAFE_DELETE(_NodeOutputArray);
    SAFE_DELETE(_NextLayerAllocTable);
    SAFE_DELETE(_NextLayerNodesSpace);
}

void LayerT::expose()
{
    bool before_ready = fullyReady();
    const size_t node_num = numNode();
    for(size_t i = 0; i < node_num; ++i)
    {
        _Nodes[i]->nodeExpose();
    }

    //   Such condition never happens that 
    // this layer is ready before this exposure but turned not ready after.
    assert(!(before_ready && !fullyReady()));

    // if turned fully ready right at this exposure
    if(!before_ready && fullyReady())
    {
        _conclude();
    }
}

struct _AllocRawInfoT {
    id_t nlnode_id; size_t pos; size_t len;
};

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
    //   The distribution of node in the next layer is determined by this layer
    // via _nextLayerNodeNum() and _mapNodeToNextLayerNode()
    if(_NextLayerNodesSpace)
        delete _NextLayerNodesSpace;
    _nextLayerNodesSpace(&_NextLayerNodesSpace);
    const size_t nlnode_num = _NextLayerNodesSpace->getSize();

    // === Generate raw allocation table ===

    // === Variables below are used to generate AllocInfoT for nlnode ===
    // The size of input of nlnodes; also is sum_groups_num
    size_t sum_input_size = 0;  
    // Sum of sizes of the output of nodes that belongs to each nlnode
    size_t *nlnode_input_sizes = new size_t[nlnode_num];
    assert(nlnode_input_sizes && "Allocation failed.");
    for(size_t i = 0; i < nlnode_num; ++i)
        nlnode_input_sizes[i] = 0;

    // === Variables below are used to generate AllocInfoT for i/o of nodes ===
    _AllocRawInfoT *output_raw_info = new _AllocRawInfoT[node_num];
    size_t sum_centers_num = 0;
    size_t *node_centers_nums = new size_t[node_num];
    assert(output_raw_info && "Allocation failed.");
    assert(node_centers_nums && "Allocation failed.");

    // Variables below are used as temp vars
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
    _NextLayerAllocTable = new AllocInfoT[nlnode_num];
    data_t *transfer_alloc;
    data_t *output_alloc;
    assert(_NodeTransferArray && _NodeOutputArray 
           && _NextLayerAllocTable && "Allocation failed.");

    data_t *now_data_tail;   // Temp var 
    id_t target_nlnode;      // Temp var, the nlnode that this node belongs
    now_data_tail = _NodeOutputArray;
    for(size_t nlnode_id = 0; nlnode_id < nlnode_num; ++nlnode_id)
    {
        _NextLayerAllocTable[nlnode_id].len = nlnode_input_sizes[nlnode_id];     
        _NextLayerAllocTable[nlnode_id].pos = now_data_tail;
        now_data_tail += nlnode_input_sizes[nlnode_id];
    }
    now_data_tail = _NodeTransferArray;
    for(size_t node_id = 0; node_id < node_num; ++node_id)
    {
        transfer_alloc= now_data_tail;
        now_data_tail += node_centers_nums[node_id];

        target_nlnode = output_raw_info[node_id].nlnode_id;
        output_alloc= _NextLayerAllocTable[target_nlnode].pos
                         + output_raw_info[node_id].pos;

        _Nodes[node_id]->concludeStepTwo(transfer_alloc, output_alloc);
    }

    // === Garbage collection ===
    delete[] nlnode_input_sizes;
    delete[] output_raw_info;
    delete[] node_centers_nums;

}

}   // namespace htm07
