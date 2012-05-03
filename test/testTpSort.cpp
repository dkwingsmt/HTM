/*
 * =====================================================================================
 *
 *       Filename:  testTpSort.cpp
 *
 *    Description:  test tp group sort.
 *
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include "TemporalPooler.h"

using namespace htm07;

int main (/* int argc, char *argv[] */)
{
    float * adj_mat = new float[25];
    for(int i = 0; i<25; ++i)
      adj_mat[i] =0;
    adj_mat[1]=4;
    adj_mat[5]=4;
    adj_mat[4]=2;
    adj_mat[20]=4;
    int size = 5;
    size_t * group_info = new size_t[5];
    size_t group_num;
    formTemperalGroup(adj_mat,size,&group_info,&group_num);
    std::cout<<"!"<<group_num<<std::endl;
    return 0;
}		
