/*
 * =====================================================================================
 *
 *       Filename:  Introspection.h
 *
 *    Description:  Friend class to all core classes, used to show test and
 *                  debug info from their private members.
 *
 * =====================================================================================
 */

#ifndef  _HTM07_INTROSPECTION_H__INC
#define  _HTM07_INTROSPECTION_H__INC

#include "Layer.h"
#include "Node.h"
#include "SpatialPooler.h"

namespace htm07 {

    typedef std::vector<OneDfloat_T> AdjMatT;

class IntrospectionT
{
public:
    int getLayerNodeNum(const LayerT *layer) {  return layer->_NumNode;     }
    NodeT const* const*getLayerNodes(const LayerT *layer)
    {   return layer->_Nodes;    }
    int getLayerNodeCenterNum(const LayerT *layer, size_t node_id)
    {   return layer->_Nodes[node_id]->_Sp->getCentersNum();    }
    const AdjMatT *getNodeSpAdjMat(const NodeT *p_node)
    {   return &p_node->_Sp->_AdjMat;    }
    const SpPatternListT *getNodeSpPatternList(const NodeT *p_node)
    {   return p_node->_Sp->_PatternList;   }
};

}   // namespace htm07


#endif   /* ----- #ifndef _HTM07_INTROSPECTION_H__INC  ----- */
