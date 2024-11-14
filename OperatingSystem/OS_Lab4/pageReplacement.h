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

#define MAX 64

enum AccessType { READ, WRITE };

// 页面置换类，负责管理页表和统计缺页次数
class PageReplacement {
public:
    PageReplacement(int numFrames);
    void displayPageTable(int pageNum, bool isPageFault, int currentAccessCount);

    int getPageFaults() const;             // 获取缺页次数
    float getPageFaultRate(int totalPages) const;        // 获取缺页率
    float getPageReplacementRate(int currentPage) const;  // 获取置换率

    std::vector<int> pageTable;    // 页表
    int pageFaults = 0;            // 缺页次数
    int pageReplacementCount = 0;  // 置换次数
    int numFrames;                 // 页框数量
};

// 页面置换算法函数声明
void FIFO(PageReplacement& pr, const std::vector<int>& sequence);
void LRU(PageReplacement& pr, const std::vector<int>& sequence);
void OPT(PageReplacement& pr, const std::vector<int>& sequence);

std::vector<int> generateSequence(int m, int e, int s, int maxPage, int length);
void selectAlgorithm(int numFrames, const std::vector<int>& sequence);

#endif // PAGEREPLACEMENT_H