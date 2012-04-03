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

namespace htm07 {

    typedef int         coord_t;
    typedef unsigned    id_t;
    typedef int         data_t;
    const int SPMAXDISTANCE = 0; // max distance to make sure two patterns are the same 

#define DATA_SIZE       (sizeof(data_t))

    struct VecT
    {
        size_t dims;
        size_t *max;
    };
    data_t computeDistance(data_t *input1,data_t* input2)
}   // namespace htm07
#endif   /* ----- #ifndef _HTM07_COMMON_H__INC  ----- */
