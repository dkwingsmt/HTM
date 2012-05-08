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
    _Dims = size->dims;
    _TotalMax = new size_t[getDimension()];
    assert(_TotalMax);
    _TotalIdProjector= new size_t[getDimension()+1];
    assert(_TotalIdProjector);
    _TotalIdProjector[0]=1;
    _SelfIdProjector= new size_t[getDimension()+1];
    assert(_SelfIdProjector);
    _SelfIdProjector[0]=1;
    _SelfMax = new size_t[getDimension()];
    assert(_SelfMax);
    _StartPos = new size_t[getDimension()];
    assert(_StartPos);
    for(size_t i = 0;i < getDimension(); ++i)
    {
        _TotalIdProjector[i+1] = _TotalIdProjector[i] * size->max[i]; 
        _SelfIdProjector[i+1] = _SelfIdProjector[i] * size->max[i];
        _TotalMax[i] = size->max[i];
        _SelfMax[i] = size->max[i];
        _StartPos[i] = 0;
    }
    _IsDerived = false;
}

bool SpaceT::getSubSpace(const VecT* start_pos, const VecT* size, 
                     SpaceT **subspace)
{
    SpaceT * tempspace = new SpaceT(start_pos,size,this);
    assert(tempspace);
    *subspace = tempspace;
    return true;
}

SpaceT::SpaceT(const SpaceT& src) :
    _Dims(src._Dims), _IsDerived(src._IsDerived)
{
    _TotalMax = new size_t[getDimension()];
    assert(_TotalMax);
    _TotalIdProjector= new size_t[getDimension()+1];
    assert(_TotalIdProjector);
    _TotalIdProjector[0]=1;
    _SelfIdProjector= new size_t[getDimension()+1];
    assert(_SelfIdProjector);
    _SelfIdProjector[0]=1;
    _SelfMax = new size_t[getDimension()];
    assert(_SelfMax);
    _StartPos = new size_t[getDimension()];
    assert(_StartPos);
    for(size_t i = 0;i < getDimension(); ++i)
    {
        _TotalIdProjector[i+1] = src._TotalIdProjector[i+1];
        _SelfIdProjector[i+1] = src._SelfIdProjector[i+1];
        _TotalMax[i] = src._TotalMax[i];
        _SelfMax[i] = src._SelfMax[i];
        _StartPos[i] = src._StartPos[i];
    }
}

SpaceT::~SpaceT()
{
    delete []_TotalMax;
    delete []_StartPos;
    delete []_TotalIdProjector;
    delete []_SelfMax;
    delete []_SelfIdProjector;
}
SpaceT::SpaceT(const VecT* start_pos, const VecT* size,SpaceT * origin)
{
    assert(size->dims==start_pos->dims);
    _Dims = origin->getDimension();
    _TotalMax = new size_t[getDimension()];
    assert(_TotalMax);
    _StartPos = new size_t[getDimension()];
    assert(_StartPos);
    _TotalIdProjector= new size_t[getDimension()+1];
    assert(_TotalIdProjector);
    _SelfMax = new size_t[getDimension()];
    _TotalIdProjector[0]=1;
    _SelfMax = new size_t[getDimension()];
    _SelfMax[0]=1;
    _SelfIdProjector = new size_t[getDimension()+1];
    assert(_SelfIdProjector);
    _SelfIdProjector[0] = 1;
    for(size_t i = 0;i < getDimension(); ++i)
    {
        _SelfMax[i]=size->max[i];
        _TotalMax[i] = origin->_TotalMax[i];
        _TotalIdProjector[i+1] = _TotalIdProjector[i] * _TotalMax[i]; 
        _StartPos[i] = start_pos->max[i];
        _SelfIdProjector[i+1] = _SelfIdProjector[i] * _SelfMax[i];
    }
    _IsDerived = true;
}

bool copyFromSpaceToSubSpace(const data_t * source, data_t * dest, const SpaceT * originspace)
{
    size_t dimension = originspace->getDimension();
    size_t * startpos = new size_t[dimension];
    size_t * endpos = new size_t[dimension];
    size_t * nowpos = new size_t[dimension];
    for(size_t i=0;i<dimension;++i)
    {
        startpos[i]=originspace->getStartPos(i);
        endpos[i]=startpos[i]+originspace->getLength(i);
        nowpos[i]=startpos[i];
    }
    int countdes = 0;
    int countsource = 0;
    while(1)
    {
        bool tempflag = true;
        for(size_t i = 0; i < dimension&&tempflag; ++i)
        {
            if(nowpos[i]+1<endpos[i])
            {
                nowpos[i]++;
                tempflag = false;
            }
            else
            {
                size_t tempaddi = i+1; 
                while(1)
                {
                    if(tempaddi == dimension)
                      break;
                    if(nowpos[tempaddi]+1<endpos[tempaddi])
                    {
                        nowpos[tempaddi]++;
                        tempflag = false;
                        for(size_t k = 0 ;k < tempaddi ; ++k)
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
        for(size_t i=0;i<dimension;++i)
        {
            countsource += nowpos[i]*originspace->getTotalIdProj(i);
        }
        dest[countdes] = source[countsource];
        countdes++;
    }
    delete []nowpos;
    delete []endpos;
    delete []startpos;

    return true;
}

}   // namespace htm07
