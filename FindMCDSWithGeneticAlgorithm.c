//
//  Func.c
//  遗传算法求解图的最小支配集
//
//  Created by Sodapig on 16/3/24.
//  Copyright © 2016年 Taozhu Ye. All rights reserved.
//

/* 为方便程序，故从0开始计数
 
 图1:
     0  1  2  3  4  5  6  7  8  9 10
 0  {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 1  {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
 2  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
 3  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 4  {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
 5  {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
 6  {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
 7  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
 8  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 9  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
 10 {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}
 
 图2:
     0  1  2  3  4  5  6  7  8
 0  {0, 0, 0, 1, 0, 0, 0, 0, 0},
 1  {0, 0, 0, 0, 1, 0, 0, 0, 0},
 2  {0, 0, 0, 1, 0, 0, 0, 0, 0},
 3  {1, 0, 1, 0, 1, 0, 0, 0, 0},
 4  {0, 1, 0, 1, 0, 1, 0, 1, 0},
 5  {0, 0, 0, 0, 1, 0, 1, 0, 1},
 6  {0, 0, 0, 0, 0, 1, 0, 0, 0},
 7  {0, 0, 0, 0, 1, 0, 0, 0, 0},
 8  {0, 0, 0, 0, 0, 1, 0, 0, 0}
 
 */


#include <stdlib.h>
#include <time.h>
#include "FindMCDSWithGeneticAlgorithm.h"
#include "FuncCreateCDS.h"

//全局变量

int const countOfDominatingSets = 12;//连通支配集数组的个数
int const countOfNodes = 40;//节点数量
int dominatingSetsArr[countOfDominatingSets][countOfNodes] = {{}};//连通支配集数组
int maxIteration = 20;//迭代次数上限
int best = countOfNodes;//迄今为止最好的结果(计算支配集中的节点个数，越小越好)
int bestOfThisTurn = countOfNodes;//本次迭代最好的结果
int bestOfThisTurnIndex = 0;//最好的支配集的下标
int bestDominatingSet[countOfNodes] = {};//最好的支配集数组
int chiasmatypyItems[countOfDominatingSets][countOfNodes+1] = {{}};//每个个体交叉的项
int countOfBlackNodesArr[countOfDominatingSets] = {};//支配集中节点数量数组
int degreeArr[countOfNodes] = {};//节点的度数组
int nodesArr[countOfNodes] = {};//所有节点的数组

//int const adjacencyMatrix[countOfNodes][countOfNodes] =
//    {{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//     {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
//     {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
//     {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//     {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
//     {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},
//     {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
//     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
//     {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//     {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}};//图1的邻接矩阵
//
//
//int const adjacencyMatrix[countOfNodes][countOfNodes] =
//    {{0, 0, 0, 1, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 1, 0, 0, 0, 0},
//     {0, 0, 0, 1, 0, 0, 0, 0, 0},
//     {1, 0, 1, 0, 1, 0, 0, 0, 0},
//     {0, 1, 0, 1, 0, 1, 0, 1, 0},
//     {0, 0, 0, 0, 1, 0, 1, 0, 1},
//     {0, 0, 0, 0, 0, 1, 0, 0, 0},
//     {0, 0, 0, 0, 1, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 1, 0, 0, 0}};//图2的邻接矩阵


int const adjacencyMatrix[countOfNodes][countOfNodes] =
{
    {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1},
    {0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,1,0,0},
    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1},
    {0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,0,1,0,0},
    {0,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,0,0,0,0,0},
    {0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
    {0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,0},
    {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,1,1},
    {0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,1,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1},
    {0,0,1,0,1,1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1},
    {0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0},
    {0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,0},
    {0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,1,0},
    {1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,1,0,0,1,1,1,0,1,1,0,0,1},
    {0,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0},
    {0,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,0,1,1},
    {0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,0},
    {0,1,1,0,0,1,1,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,1},
    {0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0}
};



//方法的声明
void selection();//选择
void crossover();//交叉
void mutation();//变异
void localOptimization();//局部优化
void printDominatingSets();//打印所有支配集
void randomArrangeCDS();//重新排列个体
void randomArrangeNodes();//重新排列节点数组
void randomArrangeNodesOfCDS(int i);//重新排列一个支配集中的节点
void findTheBestDominatingSetThisTurn();//找到本次迭代最好的结果
int isThisGraphCDS(int i);//判断是否是连通支配集
void addNodes(int i);//添加节点使之构成连通支配集
void calculateCountOfBlackNodes(int i);//计算个体中节点数量
void printResult();//计算并打印本次迭代的结果
void calculateDegree();//计算所有节点的度
void deleteRepeatedNodes(int i);//剔除重复的节点
int pickUpOneNode(int i, int j);//提取一个节点，并把后续节点前移一位
void putBackOneNode(int node, int i, int j);//放回一个节点




//遗传算法求解图的最小连通支配集
int findMCDSWithGeneticAlgorithm() {
    
    //计算出度最大的节点，把它设为生成初始种群的起始节点
    calculateDegree();
    int startNode = 0;
    for (int i = 1; i < countOfNodes; i++) {
        if (degreeArr[i] > degreeArr[startNode]) {
            startNode = i;
        }
        nodesArr[i] = i;
    }
    //生成支配集
    for (int i = 0; i < countOfDominatingSets; i++) {
        createConnectedDominatingSet(countOfNodes, adjacencyMatrix, dominatingSetsArr[i], startNode);
        calculateCountOfBlackNodes(i);
        if (isThisGraphCDS(i) == 0) {
            printf("下标为%2d的个体不是支配集\n", i);
        }
        randomArrangeNodesOfCDS(i);
    }
    printf("生成的支配集是：\n");
    printDominatingSets(countOfNodes, countOfDominatingSets, dominatingSetsArr);//打印所有支配集
    findTheBestDominatingSetThisTurn();
    best = bestOfThisTurn;
    for (int i = 0; i < countOfNodes; i++) {
        bestDominatingSet[i] = dominatingSetsArr[bestOfThisTurnIndex][i];
    }
    
    int iteration = 1;//迭代次数
    //开始迭代
    while (iteration <= maxIteration) {
        
        printf("--------------------------------------第%d次迭代--------------------------------------\n", iteration);

        selection();        //选择
        crossover();        //交叉
        mutation();        //变异
        localOptimization();        //局部优化

        printResult();      //计算并打印本次迭代的结果

        for (int i = 0; i < countOfDominatingSets; i++) {
            int notCDS = 0;
            if (isThisGraphCDS(i) == 0) {
                printf("下标为%2d的个体不是支配集\n", i);
                notCDS++;
            }
            if (notCDS > 0) {
                exit(0);
            }
        }
        iteration++;
    }
    return best;
}



//方法的实现

//计算所有节点的度
void calculateDegree() {
    for (int i = 0; i < countOfNodes; i++) {
        for (int j = 0; j < countOfNodes; j++) {
            if (i != j && adjacencyMatrix[i][j] == 1) {
                degreeArr[i]++;
            }
        }
    }
}

//计算个体中节点数量
void calculateCountOfBlackNodes(int i) {
    int ret = 0;
    for (int j = 0; j < countOfNodes && dominatingSetsArr[i][j] != -1; j++) {
        ret++;
    }
    countOfBlackNodesArr[i] = ret;
}


//选择
void selection() {
    int tmpArr[countOfDominatingSets][countOfNodes] = {};//临时数组用来存放选择结果
    int sum = 0;
    int possibilityArr[countOfDominatingSets] = {};//进行选择的可能性数组
    for (int i = 0; i < countOfDominatingSets; i++) {
        possibilityArr[i] = countOfNodes - countOfBlackNodesArr[i];
        sum += possibilityArr[i];
    }
    //保留上次迭代最优个体
    for (int i = 0; i <countOfNodes; i++) {
        tmpArr[0][i] = dominatingSetsArr[bestOfThisTurnIndex][i];
    }
    for (int i = 1; i < countOfDominatingSets; i++) {
        int randomNum = rand() % sum;//随机生成的选择因子
        int k = 0;//最终选择的结果
        while (randomNum >= possibilityArr[k] && k < countOfDominatingSets - 1) {
            randomNum -= possibilityArr[k];
            k++;
        }
        for (int j = 0; j < countOfNodes; j++) {            //将被选到的支配集赋值给临时数组
            tmpArr[i][j] = dominatingSetsArr[k][j];
        }
    }
    for (int i = 0; i < countOfDominatingSets; i++) {        //将临时数组赋值给支配集数组
        for (int j = 0; j < countOfNodes; j++) {
            dominatingSetsArr[i][j] = tmpArr[i][j];
        }
        calculateCountOfBlackNodes(i);
    }
    printf("选择的结果是：（首个个体为上次迭代最优个体之一）\n");
    printDominatingSets();//打印所有支配集
}


//交叉
void crossover() {
    randomArrangeCDS();    //分组准备交叉，两两一组
    for (int i = 0; i < countOfDominatingSets; i++) {
        randomArrangeNodesOfCDS(i);
    }
    printf("分组的结果是：\n");
    printDominatingSets();//打印所有支配集
    
    for (int i = 0; i < countOfDominatingSets; i++) {
        //找出要交叉的两组中支配集节点个数少的那一个
        int maxIndex = countOfBlackNodesArr[i];
        if (countOfBlackNodesArr[i+1] < countOfBlackNodesArr[i]) {
            maxIndex = countOfBlackNodesArr[i+1];
        }
        //生成交叉区间
        int startIndex = rand() % maxIndex;
        int endIndex = rand() % maxIndex;
        if (startIndex > endIndex) {
            int tmp = startIndex;
            startIndex = endIndex;
            endIndex = tmp;
        }
        //记录交叉的节点
        chiasmatypyItems[i][0] = endIndex - startIndex + 1;
        chiasmatypyItems[i+1][0] = endIndex - startIndex + 1;
        for (int j = 0; j < endIndex - startIndex + 1; j++) {
            chiasmatypyItems[i][j+1] = dominatingSetsArr[i][startIndex + j];
            chiasmatypyItems[i+1][j+1] = dominatingSetsArr[i+1][startIndex + j];
        }
        printf("下标为 %2d 和 %2d 的个体交叉了：下标从 %2d 到 %2d 的项\n", i, i + 1, startIndex, endIndex);
        //开始交叉
        for (int j = startIndex; j <= endIndex; j++) {
            if (dominatingSetsArr[i][j] != dominatingSetsArr[i+1][j]) {
                int tmp = dominatingSetsArr[i][j];
                dominatingSetsArr[i][j] = dominatingSetsArr[i+1][j];
                dominatingSetsArr[i+1][j] = tmp;
            }
        }
        i++;
    }
    printf("\n");
    
    for (int i = 0; i < countOfDominatingSets; i++) {
        deleteRepeatedNodes(i);//剔除重复的节点
        addNodes(i);    //判断交叉后的结果能否构成连通支配集,不是则添加一些点使它构成连通支配集
    }

    printf("交叉的结果是：\n");
    printDominatingSets();//打印所有支配集
    
    for (int i = 0; i < countOfDominatingSets; i++) {
        if (isThisGraphCDS(i) == 0) {
            printf("下标为%2d的个体不是支配集\n", i);
            exit(0);
        }
    }
}

//剔除重复的节点
void deleteRepeatedNodes(int i) {
    int j = 1;
    while (j < countOfBlackNodesArr[i]) {
        int isRepeated = 0;
        for (int k = 0; k < j; k++) {
            if (dominatingSetsArr[i][k] == dominatingSetsArr[i][j]) {
                pickUpOneNode(i, j);
                isRepeated++;
                break;
            }
        }
        if (isRepeated == 0) {
            j++;
        }
    }
    calculateCountOfBlackNodes(i);
}

//判断是否是连通支配集
int isThisGraphCDS(int i) {
    //判断是否是支配集
    for (int j = 0; j < countOfNodes; j++) {//找到灰色节点
        int isGrayNode = 1;
        for (int k = 0; k < countOfBlackNodesArr[i]; k++) {
            if (dominatingSetsArr[i][k] == j) {
                isGrayNode = 0;
                break;
            }
        }
        if (isGrayNode == 1) {
            int countOfConnectedBlackNodes = 0;
            for (int k = 0; k < countOfBlackNodesArr[i]; k++) {
                if (adjacencyMatrix[dominatingSetsArr[i][k]][j] == 1) {
                    countOfConnectedBlackNodes++;
                    break;
                }
            }
            if (countOfConnectedBlackNodes == 0) {
                return 0;
            }
        }
    }
    //判断是否连通
    int placeHolderArr[countOfNodes] = {};
    placeHolderArr[0] = 1;    //从首个节点开始，把与它连通的黑色点记录下来
    int countOfConnectedNodes = 1;//连通的黑色节点数量
    while (countOfConnectedNodes < countOfBlackNodesArr[i]) {
        int newNodeAdded = 0;
        for (int j = 0; j < countOfBlackNodesArr[i]; j++) {
            if (placeHolderArr[j] == 0) {
                for (int k = 0; k < countOfBlackNodesArr[i]; k++) {
                    if (placeHolderArr[k] == 1 && k != j && adjacencyMatrix[dominatingSetsArr[i][j]][dominatingSetsArr[i][k]] == 1) {
                        placeHolderArr[j] = 1;
                        countOfConnectedNodes++;
                        newNodeAdded++;
                        break;
                    }
                }
            }
        }
        if (newNodeAdded == 0) {
            return 0;
        }
    }
    return 1;//1代表是连通支配集
}

//添加节点使之构成连通支配集
void addNodes(int i) {
    int j = 0;
    while (isThisGraphCDS(i) == 0 && j < chiasmatypyItems[i][0]) {
        dominatingSetsArr[i][countOfBlackNodesArr[i]] = chiasmatypyItems[i][j+1];
        calculateCountOfBlackNodes(i);
        j++;
        deleteRepeatedNodes(i);
    }
}

//局部优化
void localOptimization() {
    for (int i = 0; i < countOfDominatingSets; i++) {
        int j = 0;
        int count = countOfBlackNodesArr[i];
        while (j < count && dominatingSetsArr[i][j] != -1) {
            int deletedNode = pickUpOneNode(i, j);
            if (isThisGraphCDS(i) == 0) {
                putBackOneNode(deletedNode, i, j);
                j++;
            }
        }
        calculateCountOfBlackNodes(i);
    }
    printf("局部优化的结果是：\n");
    printDominatingSets();
}

//提取一个节点，并把后续节点前移一位
int pickUpOneNode(int i, int j) {
    int node = dominatingSetsArr[i][j];
    for (int k = j; k < countOfBlackNodesArr[i] - 1; k++) {
        dominatingSetsArr[i][k] = dominatingSetsArr[i][k+1];
    }
    dominatingSetsArr[i][countOfBlackNodesArr[i] - 1] = -1;
    calculateCountOfBlackNodes(i);
    
    return node;
}

//放回一个节点
void putBackOneNode(int node, int i, int j) {
    for (int k = countOfBlackNodesArr[i]; k > j; k--) {
        dominatingSetsArr[i][k] = dominatingSetsArr[i][k-1];
    }
    dominatingSetsArr[i][j] = node;
    calculateCountOfBlackNodes(i);
}

//变异
void mutation() {
    int choosenCDSIndex = rand() % countOfDominatingSets;
    int choosenNodeIndex = rand() % countOfBlackNodesArr[choosenCDSIndex];
    printf("选中了下标为%2d的个体的下标为%2d的节点，变异的结果是：\n", choosenCDSIndex, choosenNodeIndex);
    int choosenNode = pickUpOneNode(choosenCDSIndex, choosenNodeIndex);
    
    if (isThisGraphCDS(choosenCDSIndex) == 1) {
        printf("变异成功\n");
        for (int i = 0; i < countOfBlackNodesArr[choosenCDSIndex]; i++) {
            printf(" %2d", dominatingSetsArr[choosenCDSIndex][i]);
        }
        printf("\n\n");
    } else {
        int isVariational = 0;
        randomArrangeNodes();//重新排列节点数组
        for (int i = 0; i < countOfNodes && isVariational == 0; i++) {
            int isGray = 1;
            for (int j = 0; j < countOfBlackNodesArr[choosenCDSIndex]; j++) {
                if (dominatingSetsArr[choosenCDSIndex][j] == nodesArr[i] || nodesArr[i] == choosenNode) {
                    isGray = 0;
                    break;
                }
            }
            if (isGray == 1) {
                dominatingSetsArr[choosenCDSIndex][countOfBlackNodesArr[choosenCDSIndex]] = nodesArr[i];
                countOfBlackNodesArr[choosenCDSIndex]++;
                if (isThisGraphCDS(choosenCDSIndex) == 1) {
                    printf("在添加了节点%2d后变异成功，变异的结果是：\n", nodesArr[i]);
                    for (int j = 0; j < countOfBlackNodesArr[choosenCDSIndex]; j++) {
                        printf(" %2d", dominatingSetsArr[choosenCDSIndex][j]);
                    }
                    printf("\n\n");
                    isVariational = 1;
                } else {
                    dominatingSetsArr[choosenCDSIndex][countOfBlackNodesArr[choosenCDSIndex]-1] = -1;
                    countOfBlackNodesArr[choosenCDSIndex]--;
                }
            }
        }
        if (isVariational == 0) {
            printf("变异失败\n\n");
            putBackOneNode(choosenNode, choosenCDSIndex, choosenNodeIndex);
        }
    }
}

//打印所有个体
void printDominatingSets() {
    for (int i = 0; i < countOfDominatingSets; i++) {
        for (int j = 0; j < countOfNodes && dominatingSetsArr[i][j] != -1; j++) {
            printf(" %2d", dominatingSetsArr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//计算并打印本次迭代的结果
void printResult() {
    findTheBestDominatingSetThisTurn();
    if (bestOfThisTurn < best) {
        best = bestOfThisTurn;
        for (int i = 0; i < best; i++) {
            bestDominatingSet[i] = dominatingSetsArr[bestOfThisTurnIndex][i];
        }
    }
    printf("这一次迭代最小的连通支配集的节点个数是：%d\n迄今为止最小的连通支配集的节点个数是：%d\n\n迄今为止最小的连通支配集为：\n", bestOfThisTurn, best);
    for (int i = 0; i < best; i++) {
        printf(" %2d", bestDominatingSet[i]);
    }
    printf("\n\n");
}

//重新排列支配集
void randomArrangeCDS() {
    int retArr[countOfDominatingSets][countOfNodes];
    int placeHolderArr[countOfDominatingSets];
    for (int i = 0; i < countOfDominatingSets; i++) {
        placeHolderArr[i] = 0;
    }
    for (int i = 0; i < countOfDominatingSets; i++) {
        int index = rand() % countOfDominatingSets;
        while (placeHolderArr[index] == 1) {
            index++;
            index %= countOfDominatingSets;
        }
        for (int j = 0; j < countOfNodes; j++) {
            retArr[i][j] = dominatingSetsArr[index][j];
        }
        placeHolderArr[index] = 1;
    }
    for (int i = 0; i < countOfDominatingSets; i++) {
        for (int j = 0; j < countOfNodes; j++) {
            dominatingSetsArr[i][j] = retArr[i][j];
        }
        calculateCountOfBlackNodes(i);
    }
//    printf("重新排列支配集成功\n");
}

//重新排列一个支配集中的节点
void randomArrangeNodesOfCDS(int i) {
    int retArr[countOfNodes] = {};
    int placeHolderArr[countOfNodes] = {};
    for (int j = 0; j < countOfBlackNodesArr[i]; j++) {
        int index = rand() % countOfBlackNodesArr[i];
        while (placeHolderArr[index] == 1) {
            index++;
            index %= countOfBlackNodesArr[i];
        }
        retArr[j] = dominatingSetsArr[i][index];
        placeHolderArr[index] = 1;
    }
    for (int j = 0; j < countOfBlackNodesArr[i]; j++) {
        dominatingSetsArr[i][j] = retArr[j];
    }
//    printf("重新排列一个支配集中的节点成功\n");
}

//重新排列节点数组
void randomArrangeNodes() {
    int retArr[countOfNodes] = {};
    int placeHolderArr[countOfNodes] = {};
    for (int i = 0; i < countOfNodes; i++) {
        int index = rand() % countOfNodes;
        while (placeHolderArr[index] == 1) {
            index++;
            index %= countOfNodes;
        }
        retArr[i] = nodesArr[index];
        placeHolderArr[index] = 1;
    }
    for (int i = 0; i < countOfNodes; i++) {
        nodesArr[i] = retArr[i];
    }
//    printf("重新排列节点数组成功\n");
}


//找到本次迭代最好的结果
void findTheBestDominatingSetThisTurn() {
    bestOfThisTurn = countOfNodes;
    for (int i = 0; i < countOfDominatingSets; i++) {
        if (countOfBlackNodesArr[i] < bestOfThisTurn) {
            bestOfThisTurn = countOfBlackNodesArr[i];
            bestOfThisTurnIndex = i;
        }
    }
}
