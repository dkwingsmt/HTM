/*
 * =====================================================================================
 *
 *       Filename:  Node.h
 *
 *    Description:  
 *
 * =====================================================================================
 */


#ifndef  _HTM07_NODE_H__INC
#define  _HTM07_NODE_H__INC

namespace htm07 {

class NodeT
{
public:
    NodeT(Layer *layer, VecT *node_start, id_t node_id)    {}
    ~NodeT()   {}

    void nodeExpose(data_t *input);
    
    bool learned() const;



};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_NODE_H__INC  ----- */
