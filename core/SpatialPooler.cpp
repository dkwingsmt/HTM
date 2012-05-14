/*
 * =====================================================================================
 *
 *       Filename:  SpatialPooler.cpp
 *
 *    Description:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include "SpatialPooler.h"
#define MAX_GROUP_SIZE 30
#define NTOP 2
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
            size_t temp = centers_find_more.front();
            count_for_max_size++;
            if(count_for_max_size >= MAX_GROUP_SIZE)//TODO:有错？
              break;
            center_left--;
            used[temp] = true;
            group_info_temp[temp] = group_counted;
            centers_find_more.pop();
            std::vector <size_t> savecentertemp;
            for(int fuck=0;fuck<NTOP;++fuck)//TODO:效率太低下！！！！！！！！！！！！！！！！！！
            {
                float max = 0;//TODO:表示服
                int mark = -1;              
                
                for(size_t l = fuck;l<size;++l)
                {
                    if(l != temp && used[l] == false&&adj_mat[temp*size+l] > max)
                    {
                        bool flag = true;
                        for(size_t k = 0;k<savecentertemp.size();++k)
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

SpatialPoolerT::SpatialPoolerT(data_t* input_data,size_t PatternSize)
{
    _InputData = input_data;
    _PatternList = new SpPatternListT(PatternSize);
    assert(_PatternList && "Allocation failed.");
    _Learned = false;
    _Concluded = false;
    _PreInputID = -1;
    _PatternSize = PatternSize;
    _MaxDist = 1;
    OneDfloat_T temp;
    temp.push_back(0);
    _AdjMat.push_back(temp);
}

SpPatternListT::~SpPatternListT()
{
    for (size_t i=0;i<_Patterns.size();i++)
    {
        if (_Patterns[i]!=NULL)
          delete _Patterns[i];
    }
    _Patterns.clear();
}

void SpPatternListT::push(const data_t *input,size_t data_size)
{
    assert(data_size==_PatternSize);
    data_t * temp = new data_t[data_size];
    for (size_t i=0;i<data_size;i++)
    {
        temp[i] = input[i];
    }
    _Patterns.push_back(temp);

}


void SpatialPoolerT::spLearn()
{
    bool neverAppeared = true;
    for (size_t i=0;i<_PatternList->size();i++)
    {
        int dis = computeDistance(_InputData,_PatternList->getPattern(i),_PatternSize);
       if (dis<=_MaxDist)
       {
           neverAppeared = false;
           _addTimeLine(_PreInputID,i);
           break;
       }
    }
    if (_PreInputID<0)
      _PreInputID = 0;
    else
        if (neverAppeared)
        {
            _PatternList->push(_InputData,_PatternSize);
            _addTimeLine(_PreInputID,_PatternList->size()-1);  
        } 
}
void SpatialPoolerT::spInference()
{ 
    int minID = 0; 
    int minDist = INFINITE; 
    for (size_t i=0;i<_PatternList->size();i++)
    { 
        int dist = computeDistance(_InputData,_PatternList->getPattern(i),_PatternSize); 
        if (dist<minDist) 
        { 
            minID = i;
            minDist = dist;
        }
    }
    for (size_t i=0;i<_PatternList->size();i++)
    {
        _OutputData[i] = 0;
    }
    _OutputData[minID] = 1;
}

void SpatialPoolerT::setOutputDest(data_t *pos)
{
   _OutputData = pos;
}

void SpatialPoolerT::_addTimeLine(size_t prevID, size_t currID)
{
    if (currID+1>_AdjMat.size())
    {
       for (size_t i=0;i<_AdjMat.size();i++)
         _AdjMat[i].push_back(0);
       OneDfloat_T temp;
       for (size_t i=0;i<_AdjMat[0].size();i++)
         temp.push_back(0);
       _AdjMat.push_back(temp);
    }
        
    _AdjMat[currID][prevID] ++;
    _AdjMat[prevID][currID] ++;

}


void SpatialPoolerT::sortGroup(size_t **center_group_info,size_t *group_num) const
{
    float *myadj = new float[_AdjMat.size()*_AdjMat.size()];
    for (size_t i=0;i<_AdjMat.size();i++ )
      for (size_t j=0;j<_AdjMat.size();j++)
      {
          myadj[i*_AdjMat.size()+j] = _AdjMat[i][j];        
      }
    
    formTemperalGroup(myadj,_AdjMat.size()*_AdjMat.size(), center_group_info,group_num);
    delete []myadj;
}

}   // namespace htm07
