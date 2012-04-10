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

    coord_t getTotalCoord(id_t id, size_t dim) const
    {
        return ((id/__idProjector[dim]) % getTotalLength(id));
    }
    coord_t getTotalLength(size_t id) const;
    coord_t getSelfLength(id_t id) const;
    size_t getTotalSize() const;
    size_t getSelfSize() const;

    // Same as the subspace
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

bool copyFromSpaceToSubSpace(
            const data_t * source, 
            data_t * dest, 
            const SpaceT * originspace);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
