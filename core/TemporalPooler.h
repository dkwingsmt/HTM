/*
 * =====================================================================================
 *
 *       Filename:  TemporalPooler.h
 *
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef  _HTM07_TEMPORAL_POOLER_H__INC
#define  _HTM07_TEMPORAL_POOLER_H__INC
#include <vector>
#include <cassert>
#include "Node.h"
#include "Common.h"

namespace htm07 {
void formTemperalGroup(const float* adj_mat, size_t size, std::vector<size_t>& group_info);
}
