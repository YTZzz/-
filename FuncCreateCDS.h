//
//  FuncCreateDominatingSet.h
//  遗传算法求解图的最小支配集
//
//  Created by Sodapig on 16/3/24.
//  Copyright © 2016年 Taozhu Ye. All rights reserved.
//

#ifndef FuncCreateCDS_h
#define FuncCreateCDS_h

#include <stdio.h>

//用启发式算法生成一个支配集
void createConnectedDominatingSet(int const count, int const AdjacencyMatrix[][count], int retArr[], int startNode);

#endif /* FuncCreateDominatingSet_h */
