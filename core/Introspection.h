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

class IntrospectionT
{
public:
    int getLayerNodeNum(const LayerT *layer) {  return layer->_NumNode;     }
    int getLayerNodeCenterNum(const LayerT *layer, size_t node_id)
    {   return layer->_Nodes[node_id]->_Sp->GetCentersNum();    }

};

}   // namespace htm07


#endif   /* ----- #ifndef _HTM07_INTROSPECTION_H__INC  ----- */
