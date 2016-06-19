//
//  main.c
//  遗传算法求解图的最小支配集
//
//  Created by Sodapig on 16/3/24.
//  Copyright © 2016年 Taozhu Ye. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include "FindMCDSWithGeneticAlgorithm.h"
#include <time.h>

int main(int argc, const char * argv[]) {

    int n = 10;
    int retArr[n];
    clock_t timeArr[n];
    float sumOfMCDS = 0;
    long sumOfTime = 0;

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        clock_t startTime = clock();
        retArr[i] = findMCDSWithGeneticAlgorithm();
        clock_t endTime = clock();
        timeArr[i] = endTime - startTime;

        sumOfMCDS += retArr[i];
        sumOfTime += timeArr[i];
    }
    
    printf("\n\n运行%d次的结果是：\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d 运行时间：%ld微秒\n", retArr[i], timeArr[i]);
    }
    printf("平均值是：%f，平均时间：%ld微秒，总时间：%ld微秒\n\n", sumOfMCDS/n, sumOfTime/n, sumOfTime);
    
    
    return 0;
}
