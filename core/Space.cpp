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

namespace htm07 {


SpaceT::SpaceT(const VecT *size)
{
    _TotalMax.dims = size->dims;
    _TotalMax.max = new size_t[_TotalMax.dims];
    assert(_TotalMax.max);
    this->_IdProjector= new size_t[_TotalMax.dims+1];
    assert(this->_IdProjector);
    _IdProjector[0]=1;
    this->_SelfIdProjector= new size_t[_TotalMax.dims+1];
    assert(this->_SelfIdProjector);
    _SelfIdProjector[0]=1;
    this->_SelfMax = new size_t[_TotalMax.dims];
    for(int i = 0;i < _TotalMax.dims; ++i)
    {
        _IdProjector[i+1] = _IdProjector[i] * size->max[i]; 
        _SelfIdProjector[i+1] = _SelfIdProjector[i] * size->max[i];
        _TotalMax.max[i] = size->max[i];
        _SelfMax[i] = size->max[i];
    }
    _Origin=NULL;
    this->_StartPos.dims = size->dims;
    this->_StartPos.max = NULL;
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
    delete []_TotalMax.max;
    delete []_StartPos.max;
    delete []_IdProjector;
    delete []_SelfMax;
    delete []_SelfIdProjector;
}
SpaceT::SpaceT(const VecT* start_pos, const VecT* size,SpaceT * origin)
{
    assert(size->dims==start_pos->dims);
    _TotalMax.dims = origin->_TotalMax.dims;
    this->_StartPos.dims = size->dims;
    _TotalMax.max = new size_t[_TotalMax.dims];
    assert(_TotalMax.max);
    this->_StartPos.max = new size_t[_TotalMax.dims];
    assert(this->_StartPos.max);
    this->_IdProjector= new size_t[_TotalMax.dims+1];
    assert(this->_IdProjector);
    this->_SelfMax = new size_t[_TotalMax.dims];
    _IdProjector[0]=1;
    this->_SelfMax = new size_t[_TotalMax.dims];
    _SelfMax[0]=1;
    this->_SelfIdProjector = new size_t[_TotalMax.dims+1];
    assert(this->_SelfIdProjector);
    _SelfIdProjector[0] = 1;
    for(int i = 0;i < _TotalMax.dims; ++i)
    {
        this->_SelfMax[i]=size->max[i];
        _TotalMax.max[i] = origin->_TotalMax.max[i];
        _IdProjector[i+1] = _IdProjector[i] * _TotalMax.max[i]; 
        this->_StartPos.max[i] = start_pos->max[i];
        _SelfIdProjector[i+1] = _SelfIdProjector[i] * _SelfMax[i];
    }
    _Origin = origin;//origin here is not totally copied, just a ->
}

bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace)
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
        //TODO(lrc):very very poor algorithm, left to be modified
        for(int i=0;i<dimension;++i)
        {
            countsource += nowpos[i]*originspace->_IdProjector[i];
        }
        dest[countdes] = source[countsource];
        countdes++;
    }
    delete []nowpos;
    delete []endpos;
    delete []startpos;
}
}   // namespace htm07
