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
#define MAX_GROUP_SIZE 30
#define NTOP 2
#include <iostream>
#include <vector>

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
				if(temp_sum >= max)
				{
					save_number = i;
					max = temp_sum;
					// std::cout<<"!!!"<<temp_sum<<std::endl;            
				}
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
            if(count_for_max_size >= MAX_GROUP_SIZE)//TODO:有错？
              break;
            center_left--;
            used[temp] = true;
            group_info_temp[temp] = group_counted;
            centers_find_more.pop();
            std::vector <int> savecentertemp;
            for(int fuck=0;fuck<NTOP;++fuck)//TODO:效率太低下！！！！！！！！！！！！！！！！！！
            {
                float max = 0;//TODO:表示服
                int mark = -1;              
                
                for(int l = fuck;l<size;++l)
                {
                    if(l != temp && used[l] == false&&adj_mat[temp*size+l] > max)
                    {
                        bool flag = true;
                        for(int k = 0;k<savecentertemp.size();++k)
                        {
                            if(l == savecentertemp[k])
                                flag=false;
                        }
                        if(flag)
                        {
                            mark = l;
                            max = adj_mat[temp*size+l];
                        }
                    }
                }
                if(mark != -1)
                {
                     centers_find_more.push(mark);
                     savecentertemp.push_back(mark);
                }
            }
        }
        group_counted++;
        
    }
    * group_info = group_info_temp;
    * group_num = group_counted;
    delete []used;
}
}

