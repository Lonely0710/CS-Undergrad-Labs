#include "pageReplacement.h"

// 构造函数
PageReplacement::PageReplacement(int numFrames) : numFrames(numFrames) {}

// 显示页表，并计算缺页率和置换率
void PageReplacement::displayPageTable(int pageNum, bool isPageFault, int currentAccessCount) {
    std::cout << "访问" << std::setw(3) << pageNum << " --> 内存空间 [";
    for (int page : pageTable) {
        if (page >= 0)
            std::cout << std::setw(3) << page;
        else
            std::cout << "   ";
    }
    std::cout << " ]";

    if (isPageFault) {
        std::cout << " --> 缺页中断";
    }

    // 计算并显示置换率
    if (isPageFault) {
        std::cout << " --> 置换率: " << getPageReplacementRate(currentAccessCount) << "%";
    }

    std::cout << std::endl;
}

// 获取总的缺页次数
int PageReplacement::getPageFaults() const {
    return pageFaults;
}

// 计算缺页率
float PageReplacement::getPageFaultRate(int totalPages) const {
    return (totalPages > 0) ? (static_cast<float>(pageFaults) / totalPages) * 100 : 0;
}

// 计算置换率
float PageReplacement::getPageReplacementRate(int currentAccessCount) const {
    return (currentAccessCount > 0) ? (static_cast<float>(pageReplacementCount) / currentAccessCount) * 100 : 0;
}

// FIFO算法实现
void FIFO(PageReplacement& pr, const std::vector<int>& sequence) {
    int totalPages = sequence.size();  // 总页面数
    for (int currentAccessCount = 0; currentAccessCount < totalPages; ++currentAccessCount) {
        int pageNum = sequence[currentAccessCount];
        bool isPageFault = false;

        // 检查页面是否在页表中
        auto it = std::find(pr.pageTable.begin(), pr.pageTable.end(), pageNum);
        if ( it == pr.pageTable.end()) {
            pr.pageFaults++;  // 缺页次数加一
            isPageFault = true;

            // 如果页表已满，执行页面替换
            if (pr.pageTable.size() == pr.numFrames) {
                pr.pageReplacementCount++;  // 置换次数加一
                pr.pageTable.erase(pr.pageTable.begin());  // 删除最早的页面
            }
            pr.pageTable.push_back(pageNum);  // 将当前页面放入页表
        }

        // 显示当前的页表，并计算置换率
        pr.displayPageTable(pageNum, isPageFault, currentAccessCount + 1); // +1 是因为访问次数从 1 开始
    }

    // 访问完所有页面后，计算并显示缺页率
    std::cout << "总缺页率: " << pr.getPageFaultRate(totalPages) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(totalPages) << "%" << std::endl;
}

// LRU算法实现
void LRU(PageReplacement& pr, const std::vector<int>& sequence) {
    int replace_page = 0;          // 页面替换指针
    int full = 0;                  // 是否装满所有的页框
    bool is_loss_page = false;     // 缺页标志

    // 清除页面表
    for (int n = 0; n < pr.numFrames; n++) {
        pr.pageTable.push_back(-1); // 初始化页框为空
    }

    // 记录页面的访问时间（LRU所需）
    std::vector<int> pageAccessTime(pr.numFrames, 0);
    for (int current_page = 0; current_page < sequence.size(); ++current_page) {
        int pageNum = sequence[current_page];
        bool isPageFault = false;

        // 检查页面是否在内存中
        auto it = std::find(pr.pageTable.begin(), pr.pageTable.end(), pageNum);
        int pageIndex = it - pr.pageTable.begin();

        // 页面不在内存中，发生缺页
        if (it == pr.pageTable.end()) {
            isPageFault = true;
            pr.pageFaults++;  // 缺页次数加1

            if (full < pr.numFrames) {
                // 如果页表未满，直接装入页面
                pr.pageTable[replace_page] = pageNum;
                replace_page = (replace_page + 1) % pr.numFrames;
                full++;
            } else {
                // 页表已满，执行页面替换
                int maxTime = -1, maxIndex = -1;

                // 找到最久未访问的页面
                for (int n = 0; n < pr.numFrames; ++n) {
                    if (pageAccessTime[n] > maxTime) {
                        maxTime = pageAccessTime[n];
                        maxIndex = n;
                    }
                }

                // 替换最久未访问的页面
                pr.pageReplacementCount ++;
                pr.pageTable[maxIndex] = pageNum;
                pageAccessTime[maxIndex] = 0;  // 重置该页面的访问时间
            }
        } else {
            // 页面在内存中，更新访问时间
            pageAccessTime[pageIndex] = 0;
        }

        // 更新所有页面的访问时间
        for (int i = 0; i < pr.numFrames; ++i) {
            if (i != pageIndex) {
                pageAccessTime[i]++;  // 增加其他页面的访问时间
            }
        }

        // 显示当前页表状态
        pr.displayPageTable(pageNum, isPageFault, current_page + 1);

        // 在每次页面访问后，增加访问时间的延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 等待10ms来模拟页面访问
    }
    // 访问完所有页面后，计算并显示缺页率和置换率
    std::cout << "总缺页率: " << pr.getPageFaultRate(sequence.size()) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(sequence.size()) << "%" << std::endl;
}

// OPT算法实现
void OPT(PageReplacement& pr, const std::vector<int>& sequence) {
    for (int i = 0; i < sequence.size(); ++i) {
        int pageNum = sequence[i];
        bool isPageFault = false;

        // 检查页面是否已存在于页表中
        if (std::find(pr.pageTable.begin(), pr.pageTable.end(), pageNum) == pr.pageTable.end()) {
            pr.pageFaults++;
            isPageFault = true;

            // 如果页框已满，进行页面置换
            if (pr.pageTable.size() == pr.numFrames) {
                int farthest = -1, index = -1;
                for (int j = 0; j < pr.pageTable.size(); ++j) {
                    int pos = i + 1;
                    while (pos < sequence.size() && sequence[pos] != pr.pageTable[j]) {
                        ++pos;
                    }
                    if (pos > farthest) {
                        farthest = pos;
                        index = j;
                    }
                }
                // 进行页面置换
                pr.pageTable[index] = pageNum;
                pr.pageReplacementCount++;  // 页面置换次数加1
            } else {
                // 如果页框没满，直接放入页表
                pr.pageTable.push_back(pageNum);
            }
        }

        // 传递当前访问次数
        pr.displayPageTable(pageNum, isPageFault, i + 1);
    }

    // 访问完所有页面后，计算并显示缺页率
    std::cout << "总缺页率: " << pr.getPageFaultRate(sequence.size()) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(sequence.size()) << "%" << std::endl;
}

// 生成页面访问序列
std::vector<int> generateSequence(int m, int e, int s, int maxPage, int length) {
    std::vector<int> sequence(length);
    srand(static_cast<unsigned>(time(nullptr)));

    int p = rand() % MAX;
    double t = rand() % 10 / 10.0;

    int index = 0;
    for (int i = 0; i < s && index < length; ++i) {
        for (int j = 0; j < m && index < length; ++j) {
            sequence[index++] = (p + rand() % e) % MAX;
        }
        double r = rand() % 10 / 10.0;
        if (r < t) {
            p = rand() % MAX;
        } else {
            p = (p + 1) % MAX;
        }
    }

    while (index < length) {
        sequence[index++] = rand() % MAX;
    }

    std::cout << "生成的页面访问序列为: ";
    for (int page : sequence) {
        std::cout << page << " ";
    }
    std::cout << std::endl;

    return sequence;
}

// 选择并运行页面置换算法
void selectAlgorithm(int numFrames, const std::vector<int>& sequence) {
    int choice;
    std::cout << "\n选择页面置换算法:\n"
                 "1. 先进先出算法(FIFO)\n"
                 "2. 最近最久未使用算法(LRU)\n"
                 "3. 最佳置换算法(OPT)\n";
    std::cout << "输入你的选择 (1/2/3): ";
    std::cin >> choice;

    PageReplacement pr(numFrames);
    switch (choice) {
        case 1:
            std::cout << "\n<----------FIFO页面置换算法---------->\n";
            FIFO(pr, sequence);
            break;
        case 2:
            std::cout << "\n<----------LRU页面置换算法---------->\n";
            LRU(pr, sequence);
            break;
        case 3:
            std::cout << "\n<----------OPT页面置换算法---------->\n";
            OPT(pr, sequence);
            break;
        default:
            std::cout << "无效选择。\n";
    }

    std::cout << "总缺页次数: " << pr.pageFaults << std::endl;
}