/*
 * =====================================================================================
 *
 *       Filename:  Space.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "Space.h"
#include <cassert>

namespace htm07 {


SpaceT::SpaceT(const VecT *max)
{
    this->max.dims = max->dims;
    this->max.max = new size_t[this->max.dims];
    assert(this->max.max);
    this->__idProjector= new size_t[this->max.dims];
    assert(this->__idProjector);
    __idProjector[0]=1;
    this->_SelfMax = new size_t[this->max.dims];
    for(int i = 0;i < this->max.dims; ++i)
    {
        __idProjector[i+1] = __idProjector[i] * max->max[i]; 
        this->max.max[i] = max->max[i];
        _SelfMax[i] = max->max[i];
    }
    _Origin=NULL;
    this->_StartPos.dims=max->dims;
    this->_StartPos.max=NULL;
}

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
        return _SelfMax[id];
    else
        return -1;//to be modified
}
size_t SpaceT::getSize() const
{
    return __idProjector[max.dims+1];
}

bool SpaceT::getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace)
{
    SpaceT * tempspace = new SpaceT(start_pos,size,this);
    assert(tempspace);
    *subspace = tempspace;
    return true;
}
SpaceT::~SpaceT()
{
    delete []max.max;
    delete []_StartPos.max;
    delete []__idProjector;
    delete []_SelfMax;
    delete []_SelfidProjector;
}
SpaceT::SpaceT(const VecT* start_pos, const VecT* size,SpaceT * origin)
{
    assert(size->dims==start_pos->dims);
    this->max.dims = origin->max.dims;
    this->_StartPos.dims = size->dims;
    this->max.max = new size_t[max.dims];
    assert(this->max.max);
    this->_StartPos.max = new size_t[max.dims];
    assert(this->_StartPos.max);
    this->__idProjector= new size_t[this->max.dims];
    assert(this->__idProjector);
    this->_SelfMax = new size_t[max.dims];
    __idProjector[0]=1;
    this->_SelfMax = new size_t[max.dims];
    _SelfMax[0]=1;
    for(int i = 0;i < max.dims; ++i)
    {
        this->_SelfMax[i]=size->max[i];
        __idProjector[i+1] = __idProjector[i] * max.max[i]; 
        this->max.max[i] = origin->max.max[i];
        this->_StartPos.max[i] = start_pos->max[i];
        _SelfidProjector[i+1] = _SelfidProjector[i] * _SelfMax[i];
    }
    _Origin = origin;
}

}   // namespace htm07
