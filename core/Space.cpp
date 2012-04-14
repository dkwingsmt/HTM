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

#include <iostream>




#include "Layer.h"

namespace htm07 {


SpaceT::SpaceT(const VecT *max)
{
    this->max.dims = max->dims;
    this->max.max = new size_t[this->max.dims];
    assert(this->max.max);
    this->__idProjector= new size_t[this->max.dims+1];
    assert(this->__idProjector);
    __idProjector[0]=1;
    this->_SelfidProjector= new size_t[this->max.dims+1];
    assert(this->_SelfidProjector);
    _SelfidProjector[0]=1;
    this->_SelfMax = new size_t[this->max.dims];
    for(int i = 0;i < this->max.dims; ++i)
    {
        __idProjector[i+1] = __idProjector[i] * max->max[i]; 
        _SelfidProjector[i+1] = _SelfidProjector[i] * max->max[i];
        this->max.max[i] = max->max[i];
        _SelfMax[i] = max->max[i];
    }
    _Origin=NULL;
    this->_StartPos.dims=max->dims;
    this->_StartPos.max=NULL;
}

coord_t SpaceT::getTotalLength(id_t id) const
{
    if(id < this->max.dims)
        return this->max.max[id];
    assert(0);
    return -1;//to be modified
}

coord_t SpaceT::getSelfLength(id_t id) const
{
    if(id < max.dims)
    {
        if(isDerived())  
            return _SelfMax[id];
        else
            return max.max[id];
    }
    assert(0);
    return -1;//to be modified
}
size_t SpaceT::getTotalSize() const
{
    return __idProjector[max.dims];
}

size_t SpaceT::getSelfSize() const
{
    if(!isDerived())
        return __idProjector[max.dims];
    else
        return _SelfidProjector[max.dims];
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
    this->__idProjector= new size_t[this->max.dims+1];
    assert(this->__idProjector);
    this->_SelfMax = new size_t[max.dims];
    __idProjector[0]=1;
    this->_SelfMax = new size_t[max.dims];
    _SelfMax[0]=1;
    this->_SelfidProjector = new size_t[this->max.dims+1];
    assert(this->_SelfidProjector);
    _SelfidProjector[0] = 1;
    for(int i = 0;i < max.dims; ++i)
    {
        this->_SelfMax[i]=size->max[i];
        this->max.max[i] = origin->max.max[i];
        __idProjector[i+1] = __idProjector[i] * max.max[i]; 
        this->_StartPos.max[i] = start_pos->max[i];
        _SelfidProjector[i+1] = _SelfidProjector[i] * _SelfMax[i];
    }
    _Origin = origin;//origin here is not totally copied, just a ->
}

bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace, LayerT *layer)
{
    size_t dimension = originspace->getDimension();
    size_t * startpos = new size_t[dimension];
    size_t * endpos = new size_t[dimension];
    size_t * nowpos = new size_t[dimension];
    for(int i=0;i<dimension;++i)
    {
        startpos[i]=originspace->_StartPos.max[i];
        endpos[i]=startpos[i]+originspace->_SelfMax[i];
        nowpos[i]=startpos[i];
    }
    int countdes = 0;
    int countsource = 0;
    while(1)
    {
        bool tempflag = true;
        for(int i = 0; i < dimension&&tempflag; ++i)
        {
            if(nowpos[i]+1<endpos[i])
            {
                nowpos[i]++;
                tempflag = false;
            }
            else
            {
                int tempaddi = i+1; 
                while(1)
                {
                    if(tempaddi == dimension)
                      break;
                    if(nowpos[tempaddi]+1<endpos[tempaddi])
                    {
                        nowpos[tempaddi]++;
                        tempflag = false;
                        for(int k = 0 ;k < tempaddi ; ++k)
                        {
                            nowpos[k] = startpos[k];
                        }//TODO(lrc):poor algorithm
                        break;
                    }
                    else
                    {
                        tempaddi++;
                    }
                }
            }
        }
        if(tempflag == true)
          break;
        countsource = 0;
        for(int i=0;i<dimension;++i)//TODO(lrc):very very poor algorithm, left to be modified
        {
            countsource += nowpos[i]*originspace->__idProjector[i];
        }
        dest[countdes] = source[countsource];
        countdes++;
    }
    delete []nowpos;
    delete []endpos;
    delete []startpos;
}
}   // namespace htm07
