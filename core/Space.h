/*
 * =====================================================================================
 *
 *       Filename:  Space.h
 *
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef  _HTM07_SPACE_H__INC
#define  _HTM07_SPACE_H__INC

#include <stdlib.h>
#include "Common.h"

namespace htm07 {

class IntrospectionT;


class LayerT;

class SpaceT
{
    friend class IntrospectionT;
public:
    // New SpaceT from a given VecT as size of each dimensions
    SpaceT(const VecT *size);
    // A subspace from an exsisted space within the given region
    SpaceT(const VecT* start_pos, const VecT* size,SpaceT * origin);
    ~SpaceT();

    inline coord_t getTotalCoord(id_t id, size_t dim) const;
    inline coord_t getTotalLength(size_t id) const;
    inline coord_t getSelfLength(id_t id) const;
    inline size_t getTotalSize() const;
    inline size_t getSelfSize() const;

    // Same as the subspace
    bool getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace);

    bool isDerived() const{return !_Origin;}
    size_t getDimension() const {return _TotalMax.dims;}
private:
    VecT _TotalMax;
    size_t *_SelfMax;
    size_t *_IdProjector;
    VecT _StartPos;
    SpaceT *_Origin;
    size_t *_SelfIdProjector;
    friend bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace);
};

coord_t SpaceT::getTotalCoord(id_t id, size_t dim) const
{
    return ((id/_IdProjector[dim]) % getTotalLength(dim));
}

coord_t SpaceT::getTotalLength(id_t id) const
{
    if(id < _TotalMax.dims)
        return _TotalMax.max[id];
    assert(0);
    return -1;//to be modified
}

coord_t SpaceT::getSelfLength(id_t id) const
{
    if(id < _TotalMax.dims)
    {
        if(isDerived())  
            return _SelfMax[id];
        else
            return _TotalMax.max[id];
    }
    assert(0);
    return -1;//to be modified
}
size_t SpaceT::getTotalSize() const
{
    return _IdProjector[_TotalMax.dims];
}

size_t SpaceT::getSelfSize() const
{
    if(!isDerived())
        return _IdProjector[_TotalMax.dims];
    else
        return _SelfIdProjector[_TotalMax.dims];
}

bool copyFromSpaceToSubSpace(
            const data_t * source, 
            data_t * dest, 
            const SpaceT * originspace);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
