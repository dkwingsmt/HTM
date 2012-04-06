/*
 * =====================================================================================
 *
 *       Filename:  Common.h
 *
 *    Description:  
 *
 * =====================================================================================
 */


#ifndef  _HTM07_COMMON_H__INC
#define  _HTM07_COMMON_H__INC

#include <cstdlib>

namespace htm07 {

    typedef int         coord_t;
    typedef unsigned    id_t;
    typedef int         data_t;
    const data_t INFINITE = 200000000;
    const data_t SPMAXDISTANCE = 0; // max distance to make sure two patterns are the same 

    struct VecT
    {
        size_t dims;
        size_t *max;
    };
    data_t computeDistance(const data_t* input1,const data_t* input2,size_t size1,size_t size2);
    void initializeVec(VecT *vec, size_t size);

}   // namespace htm07
#endif   /* ----- #ifndef _HTM07_COMMON_H__INC  ----- */
