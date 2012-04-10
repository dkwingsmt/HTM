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
    inline coord_t getTotalLength(id_t id) const;
    inline coord_t getSelfLength(id_t id) const;
    inline size_t getTotalSize() const;
    inline size_t getSelfSize() const;

    bool getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace);

    bool isDerived() const{return !_Origin;}
    size_t getDimension() const {return max.dims;}
private:
    VecT max;
    size_t *_SelfMax;
    size_t *__idProjector;
    VecT _StartPos;
    SpaceT *_Origin;
    size_t *_SelfidProjector;
    friend bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace);
};

coord_t SpaceT::getTotalCoord(id_t id, size_t dim) const
{
    return ((id/__idProjector[dim]) % getTotalLength(id));
}

coord_t SpaceT::getTotalLength(id_t id) const
{
    if(id <= max.dims)
        return max.max[id];
    else
        return -1;//to be modified
}

coord_t SpaceT::getSelfLength(id_t id) const
{
    if(id <= max.dims)
    {
        if(isDerived())  
            return _SelfMax[id];
        else
            return max.max[id];
    }
    else
        return -1;//to be modified
}
size_t SpaceT::getTotalSize() const
{
    return __idProjector[max.dims+1];
}
size_t SpaceT::getSelfSize() const
{
    if(!isDerived())
        return __idProjector[max.dims+1];
    else
        return _SelfidProjector[max.dims+1];
}

bool copyFromSpaceToSubSpace(
            const data_t * source, 
            data_t * dest, 
            const SpaceT * originspace);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
