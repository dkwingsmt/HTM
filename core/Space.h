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

class SubSpaceT;

class SpaceT
{
public:
    SpaceT(const VecT *max);
    ~SpaceT();

    coord_t getCoord(id_t id, size_t dim) const;
    coord_t getMaxCoord(id_t id) const;
    size_t getSize() const;

    bool getSubSpace(const VecT* start_pos, const VecT* size, 
                     SubSpaceT **subspace);
private:
    VecT max;
    size_t *__idProjector;
    friend class SubSpaceT;
};

class SubSpaceT
{
public:
    SubSpaceT(){}
    ~SubSpaceT() ;
    bool initial(const VecT* start_pos, const VecT* size,SpaceT * origin);
private:
    VecT max;
    VecT _StartPos;
    SpaceT * _Origin;

};

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
