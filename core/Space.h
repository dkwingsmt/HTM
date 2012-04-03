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
    SpaceT(VecT *max) {}
    ~Space() {}
};

class SubSpaceT
{
public:
    SubSpaceT(SpaceT* space) {}
    ~SubSpaceT() {}
}

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_SPACE_H__INC  ----- */
