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
    adj-mat[20]=4;
    size = 5;
    int * group_info = new int[5];
    int group_num;
    formTemperalGroup(adj_mat,size,&group_info,&group_num);
    std::cout<<"!"<<group_num<<endl;
    return 0;
}		
