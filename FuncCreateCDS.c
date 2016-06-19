//
//  FuncCreateDominatingSet.c
//  遗传算法求解图的最小支配集
//
//  Created by Sodapig on 16/3/24.
//  Copyright © 2016年 Taozhu Ye. All rights reserved.
//

#include "FuncCreateCDS.h"
#include <stdlib.h>
#include <time.h>

//用启发式算法生成一个支配集
void createConnectedDominatingSet(int const count, int const AdjacencyMatrix[][count], int retArr[], int startNode) {
    
    int dyedArr[count];    //染色数组 0代表白色 1代表灰色 2代表黑色
    for (int i = 0; i < count; i++) {
        dyedArr[i] = 0;
    }
    dyedArr[startNode] = 1;    //把初始点染成灰的
    int countOfGray = 1;    //灰色点的个数
    int countOfBlcak = 0;   //黑色点的个数
    
    while (countOfGray + countOfBlcak < count) {
        //随机选一个灰色的点，把它染黑
        int n = rand() % countOfGray;   //选中的是第几个灰色的点，从0开始计数
        int num = 0;
        int j = 0;
        for (j = 0; j < count; j++) {
            if (dyedArr[j] == 1) {
                if (num == n) {
                    dyedArr[j] = 2;
                    countOfBlcak++;
                    countOfGray--;
                    break;
                }
                num++;
            }
        }
        for (int k = 0; k < count; k++) {        //把与j点相连的白色点染灰
            if (AdjacencyMatrix[j][k] == 1 && dyedArr[k] == 0) {
                dyedArr[k] = 1;
                countOfGray++;
            }
        }
    }
    int k = 0;
    for (int j = 0; j < count; j++) {    //把结果赋值进retArr
        if (dyedArr[j] == 2) {
            retArr[k] = j;
            k++;
        }
    }
    while (k < count) {
        retArr[k] = -1;
        k++;
    }
}