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

#define DATA_SIZE       (sizeof(data_t))

    struct VecT
    {
        size_t dims;
        size_t *max;
    };
    void initializeVec(VecT *vec, size_t size);

}   // namespace htm07

#endif   /* ----- #ifndef _HTM07_COMMON_H__INC  ----- */
