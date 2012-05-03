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

#include <iostream>

namespace htm07 {

template < typename T >
    inline T __minBinary(const T &lhs, const T &rhs)
    {
        return (lhs < rhs) ? lhs : rhs;
    }

NodeT::NodeT(LayerT *layer, id_t node_id, const AllocInfoT &alloc_info) :
    _Concluded(false)
{
    _Layer = layer;
    _Id = node_id;
    size_t dims = _Layer->dims();
    _InputData = alloc_info.pos;
    _InputLen = alloc_info.len;

    _Sp = new SpatialPoolerT(alloc_info.len);
    assert(_Sp && "Allocation failed.");
}

NodeT::~NodeT()
{
    delete _Sp;
}

void NodeT::nodeExpose()
{
    // Transform complete input data to data block
    if(!concluded())
    {
        bool ready_to_conclude_before = readyToConclude();
        assert(!_Sp->learned());
        _Sp->spLearn(_InputData, _InputLen);
        assert(!(ready_to_conclude_before && !readyToConclude()));
        if(!ready_to_conclude_before && readyToConclude())
        {
            _Layer->nodeReadyToConclude(_Id);
        }
    }
    else
    {
        assert(!_Sp->learned());
        //assert(_Tp);
        _Sp->spInference(_InputData, _InputLen);
        // tp output
    }
}

void NodeT::concludeStepOne()
{
    size_t *sort_result = NULL;
    size_t groups_num = 0;
    _Sp->sortGroup(&sort_result, &groups_num);
    // tp = new Tp(d, r)
}

void NodeT::concludeStepTwo(const AllocInfoT& sp_output_alloc,
                            const AllocInfoT& tp_output_alloc)
{
    _Sp->setOutputDest(sp_output_alloc);
    // store tp_output_alloc
    _Sp->setConcluded();
    _Concluded = true;
}

}   // namespace htm07
