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

    inline coord_t getLength(size_t dim) const;
    inline size_t getSize() const;
    inline coord_t getCoord(id_t id, size_t dim) const;
    inline size_t getDimension() const {return _Dims;}
    inline size_t getIdProj(size_t dim) const;
    inline size_t getStartPos(size_t dim) const;

    inline coord_t getTotalCoord(id_t id, size_t dim) const;
    inline coord_t getTotalLength(size_t dim) const;
    inline size_t getTotalSize() const;
    inline size_t getTotalIdProj(size_t dim) const;

    bool getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace);

    // Returns if this space is a subspace of another one. True for a subspace.
    bool isDerived() const      {return _IsDerived;}
private:
    size_t _Dims;
    size_t *_TotalMax;
    size_t *_SelfMax;
    size_t *_TotalIdProjector;
    size_t *_StartPos;
    size_t *_SelfIdProjector;
    bool _IsDerived;
};
size_t SpaceT::getIdProj(size_t dim) const
{
    assert(dim < getDimension() + 1 && "Dimension exceeded limit.");
    return _SelfIdProjector[dim];
}
size_t SpaceT::getTotalIdProj(size_t dim) const
{
    assert(dim < getDimension() + 1 && "Dimension exceeded limit.");
    return _TotalIdProjector[dim];
}

size_t SpaceT::getStartPos(size_t dim) const
{
    assert(dim < getDimension() && "Dimension exceeded limit.");
    return _StartPos[dim];
}

coord_t SpaceT::getCoord(id_t id, size_t dim) const
{
    assert(dim < getDimension() && "Dimension exceeded limit.");
    return ((id/_SelfIdProjector[dim]) % getLength(dim));
}

coord_t SpaceT::getTotalCoord(id_t id, size_t dim) const
{
    assert(dim < getDimension() && "Dimension exceeded limit.");
    return ((id/_TotalIdProjector[dim]) % getTotalLength(dim));
}

coord_t SpaceT::getTotalLength(size_t dim) const
{
    assert(dim < getDimension() && "Dimension exceeded limit.");
    return _TotalMax[dim];
}

coord_t SpaceT::getLength(size_t dim) const
{
    assert(dim < getDimension() && "Dimension exceeded limit.");
    return _SelfMax[dim];
}
size_t SpaceT::getTotalSize() const
{
    return _TotalIdProjector[getDimension()];
}

size_t SpaceT::getSize() const
{
    return _SelfIdProjector[getDimension()];
}

bool copyFromSpaceToSubSpace(
            const data_t * source, 
            data_t * dest, 
            const SpaceT * originspace);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
