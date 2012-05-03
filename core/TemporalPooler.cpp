/*
 * =====================================================================================
 *
 *       Filename:  TemporalPooler.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "TemporalPooler.h"
#include <queue>
#define MAX_GROUP_SIZE 30;
#define NTOP 2;
#include <iostream>

namespace htm07 {
void formTemperalGroup(const float* adj_mat, size_t size, size_t ** group_info,size_t * group_num)
    //长度size = adj_mat centers number, 
{
    bool * used = new bool[size];
    size_t * group_info_temp = new size_t[size];
    for(size_t   i = 0;i<size;++i)
    {
        used[i] = false;
    }
    size_t group_counted = 0;
    int center_left = size;
    while(center_left>0)
    {
        //step 1: find most connected
        int save_number;
        float max = -1;
        for(size_t i = 0; i < size; ++i)
        {

            float temp_sum = 0;
            if(used[i] == false)
            {
                //int count_adj;
                for(size_t j = 0;j<size; ++j)
                {
                    if(j != i&& used[j] == false)
                    {
                        temp_sum += adj_mat[i*size+j];
                    }
                }
            }
            if(temp_sum >= max)
            {
                save_number = i;
                max = temp_sum;
           // std::cout<<"!!!"<<temp_sum<<std::endl;            
            }
        }
        //step 2: add connected nodes

        std::queue<size_t> centers_find_more;
        centers_find_more.push(save_number);
        size_t count_for_max_size = 0;
        while(centers_find_more.size() != 0)
        {
            int temp = centers_find_more.front();
            count_for_max_size++;
            if(count_for_max_size >= 30)//TODO:有错？
              break;
            center_left--;
            used[temp] = true;
            group_info_temp[temp] = group_counted;
            centers_find_more.pop();
            int max1number,max2number;//TODO:已经整合了两个最大的进去，目前效率较高，以后可能改成一个排序之后可以取前NTOP个
            int to_be_inline = 0;
            for(size_t j = 0;j < size; ++j)
            {
                size_t i = temp;
                float max1 = -1;//TODO: 不含有负数？
                float max2 = -2;
                if(j != i && used[j] == false)
                {
                    if(adj_mat[i*size+j] >= max1)
                    {
                        max2number = max1number;
                        max2 = max1;
                        max1number = j;
                        max1 = adj_mat[i*size+j];
                        to_be_inline++;
                    }
                    else if(adj_mat[i*size+j] >= max2)
                    {
                        max1number = j;
                        max1 = adj_mat[i*size+j];
                        to_be_inline++;
                    }
                }
            }
            if(to_be_inline == 1)
            {
                centers_find_more.push(max1number);
            }
            else if(to_be_inline >= 2)
            {
                centers_find_more.push(max1number);
                centers_find_more.push(max2number);
            }
        }
        group_counted++;
        
    }
    * group_info = group_info_temp;
    group_num = &group_counted;
    delete []used;
}
}

