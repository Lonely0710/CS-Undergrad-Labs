#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <unordered_map>

#define MAX 64 // 最大页号

// 页面置换类，负责管理页表和统计缺页次数
class PageReplacement {
public:
    PageReplacement(int numFrames);
    void displayPageTable(int pageNum, bool isPageFault, int currentAccessCount);

    float getPageFaultRate(int totalPages) const;        // 获取缺页率
    float getPageReplacementRate(int currentPage) const;  // 获取置换率

    std::vector<int> pageTable;         // 页表
    std::vector<int> useBit;            // 使用位数组
    int pageFaults = 0;                 // 缺页次数
    int pageReplacementCount = 0;       // 置换次数
    int numFrames;                      // 页框数量
    int hand;                           // 时钟指针

};

// 页面置换算法函数声明
void FIFO(PageReplacement& pr, const std::vector<int>& sequence);
void LRU(PageReplacement& pr, const std::vector<int>& sequence);
void OPT(PageReplacement& pr, const std::vector<int>& sequence);
void CLOCK(PageReplacement& pr, const std::vector<int>& sequence);
void CLOCKpro(PageReplacement& pr, const std::vector<int>& sequence);

std::vector<int> generateSequence(int m, int e, int s, int length);
void selectAlgorithm(int numFrames, const std::vector<int>& sequence);
std::vector<int> choosePageSequence();

#endif // PAGEREPLACEMENT_H