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
    float * adj_mat = new float[49];
    for(int i = 0; i<49; ++i)
      adj_mat[i] =0;
    adj_mat[2]=4;//0,2
    adj_mat[14]=4;
    adj_mat[4]=3;//0,4
    adj_mat[28]=3;
	adj_mat[12]=2;//1,5
	adj_mat[36]=2;
	adj_mat[43]=7;//1,6
	adj_mat[13]=7;
	adj_mat[7]=1;//1,0
	adj_mat[1]=1;
    int size = 7;
    size_t * group_info = new size_t[7];
    size_t group_num;
    formTemperalGroup(adj_mat,size,&group_info,&group_num);
    std::cout<<"!"<<group_num<<std::endl;
    for(int i = 0;i<7;++i)
    {
        std::cout<<"the "<<i<<" is " <<group_info[i]<<" !"<<std::endl;
    }
    return 0;
}		
