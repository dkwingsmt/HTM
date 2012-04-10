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

class SpaceT
{
public:
    // New SpaceT from a given VecT as size of each dimensions
    SpaceT(const VecT *max);
    // A subspace from an exsisted space within the given region
    SpaceT(const VecT* start_pos, const VecT* size,SpaceT * origin);
    ~SpaceT();

    inline coord_t getTotalCoord(id_t id, size_t dim) const;
    inline coord_t getTotalLength(size_t dim) const;
    inline coord_t getSelfLength(size_t dim) const;
    inline size_t getTotalSize() const;
    inline size_t getSelfSize() const;

    bool getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace);

    bool isDerived() const      {return !_Origin;}
    size_t getDimension() const {return max.dims;}
private:
    VecT max;
    size_t *_SelfMax;
    size_t *_IdProjector;
    VecT _StartPos;
    SpaceT *_Origin;
    size_t *_SelfIdProjector;

    friend bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace);
};

coord_t SpaceT::getTotalCoord(id_t id, size_t dim) const
{
    return ((id/_IdProjector[dim]) % getTotalLength(id));
}

coord_t SpaceT::getTotalLength(size_t dim) const
{
    if(dim <= max.dims)
        return max.max[dim];
    else
        return -1;//to be modified
}

coord_t SpaceT::getSelfLength(size_t dim) const
{
    if(dim <= max.dims)
    {
        if(isDerived())  
            return _SelfMax[dim];
        else
            return max.max[dim];
    }
    else
        return -1;//to be modified
}

size_t SpaceT::getTotalSize() const
{
    return _IdProjector[max.dims+1];
}

size_t SpaceT::getSelfSize() const
{
    if(!isDerived())
        return _IdProjector[max.dims+1];
    else
        return _SelfIdProjector[max.dims+1];
}

bool copyFromSpaceToSubSpace(
            const data_t * source, 
            data_t * dest, 
            const SpaceT * originspace);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
