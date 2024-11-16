#include "pageReplacement.h"

// 构造函数
PageReplacement::PageReplacement(int numFrames) : numFrames(numFrames), pageTable(), useBit(numFrames, -1), hand(0) {}

// 显示页表，并计算缺页率和置换率
void PageReplacement::displayPageTable(int pageNum, bool isPageFault, int currentAccessCount) {
    std::cout << "访问" << std::setw(3) << pageNum << " --> 内存空间 [";

    // 打印当前页表中的页面
    for (int i = 0; i < numFrames; ++i) {
        if (i < pageTable.size() && pageTable[i] != -1) {
            // 页表中有页面
            std::cout << std::setw(3) << pageTable[i];
        } else {
            // 页表中没有更多页面，用空格填充
            std::cout << "   ";
        }
    }
    std::cout << " ]";

    // 只有在useBit已被修改过（即非初始状态）才打印标志位
    bool printFlags = false;
    for (int i = 0; i < numFrames; ++i) {
        if (useBit[i] != -1) {
            printFlags = true;
            break;
        }
    }

    if (printFlags) {
        // 打印标志位，只有标志位为1的页面才打印
        std::cout << " 标志位 [";
        for (int i = 0; i < numFrames; ++i) {
            if (useBit[i] == 1) {
                std::cout << std::setw(3) << "1 ";  // 页面已访问
            } else if (pageTable[i] != -1) {
                std::cout << std::setw(3) << "0 ";  // 页面未访问
            } else {
                std::cout << "   ";  // 空位
            }
        }
        std::cout << "]";
    }

    if (isPageFault) {
        std::cout << " --> 缺页中断";
    }

    // 计算并显示置换率
    if (isPageFault) {
        std::cout << " --> 置换率: " << getPageReplacementRate(currentAccessCount) << "%";
    }

    std::cout << std::endl;
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
    int replaceIndex = 0;  // 用于记录当前需要替换的位置（FIFO策略）

    for (int currentAccessCount = 0; currentAccessCount < totalPages; ++currentAccessCount) {
        int pageNum = sequence[currentAccessCount];
        bool isPageFault = false;

        // 检查页面是否在页表中
        auto it = std::find(pr.pageTable.begin(), pr.pageTable.end(), pageNum);
        if (it == pr.pageTable.end()) {
            pr.pageFaults++;  // 缺页次数加一
            isPageFault = true;

            // 如果页表未满，则直接添加
            if (pr.pageTable.size() < pr.numFrames) {
                pr.pageTable.push_back(pageNum);
            } else {
                // 页表已满，直接替换掉 replaceIndex 指向的位置
                pr.pageReplacementCount++;  // 置换次数加一
                pr.pageTable[replaceIndex] = pageNum;  // 替换页面
                replaceIndex = (replaceIndex + 1) % pr.numFrames;  // 更新替换索引（循环队列）
            }
        }

        // 显示当前的页表，并计算置换率
        pr.displayPageTable(pageNum, isPageFault, currentAccessCount + 1);
    }

    // 访问完所有页面后，计算并显示缺页率
    std::cout << "总缺页率: " << pr.getPageFaultRate(totalPages) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(totalPages) << "%" << std::endl;
}

// LRU算法实现
void LRU(PageReplacement& pr, const std::vector<int>& sequence) {
    int full = 0; // 页表是否已满

    // 清除页面表并初始化访问时间
    pr.pageTable.clear();
    std::vector<int> pageAccessTime(pr.numFrames, 0);

    for (int current_page = 0; current_page < sequence.size(); ++current_page) {
        int pageNum = sequence[current_page];
        bool isPageFault = false;

        // 检查页面是否在内存中
        auto it = std::find(pr.pageTable.begin(), pr.pageTable.end(), pageNum);
        int pageIndex = (it != pr.pageTable.end()) ? std::distance(pr.pageTable.begin(), it) : -1;

        if (it == pr.pageTable.end()) {
            // 页面不在内存中，发生缺页
            isPageFault = true;
            pr.pageFaults++;

            if (full < pr.numFrames) {
                // 页表未满，直接添加页面
                pr.pageTable.push_back(pageNum);
                pageAccessTime[full] = 0; // 初始化访问时间
                full++;
            } else {
                // 页表已满，找到最久未访问的页面
                int maxTime = -1, maxIndex = -1;
                for (int n = 0; n < pr.numFrames; ++n) {
                    if (pageAccessTime[n] > maxTime) {
                        maxTime = pageAccessTime[n];
                        maxIndex = n;
                    }
                }

                // 替换最久未使用的页面
                pr.pageReplacementCount++;
                pr.pageTable[maxIndex] = pageNum;
                pageAccessTime[maxIndex] = 0; // 重置访问时间
            }
        } else {
            // 页面在内存中，更新其访问时间
            pageAccessTime[pageIndex] = 0;
        }

        // 更新所有页面的访问时间
        for (int i = 0; i < full; ++i) {
            if (pr.pageTable[i] != pageNum) {
                pageAccessTime[i]++;
            }
        }

        // 显示当前页表状态
        pr.displayPageTable(pageNum, isPageFault, current_page + 1);
    }

    // 显示统计结果
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

// 简单Clock置换算法实现
void CLOCK(PageReplacement& pr, const std::vector<int>& sequence) {
    int accessCount = 0;
    std::unordered_map<int, int> pageToIndex;  // 用于快速查找页面是否在页表中

    pr.pageTable.assign(pr.numFrames, -1);  // 初始化页表，-1 表示无效页面

    for (int page : sequence) {
        ++accessCount;
        bool isPageFault = false;

        // 检查页面是否在页表中
        auto it = pageToIndex.find(page);
        if (it != pageToIndex.end()) {
            // 页面已存在，设置使用位
            pr.useBit[it->second] = 1; // 标记该页面被访问过
            pr.displayPageTable(page, false, accessCount);  // false表示没有缺页
            continue;
        }

        // 页面不在页表中，发生缺页
        pr.pageFaults++;
        while (true) {
            // 检查当前时钟指针位置的使用位
            if (pr.useBit[pr.hand] == 0) {
                // 置换页面
                if (pr.pageTable[pr.hand] != -1) {
                    pageToIndex.erase(pr.pageTable[pr.hand]); // 从映射中移除被置换的页面
                    pr.pageReplacementCount++;  // 置换次数加一
                }

                // 替换为新页面
                pr.pageTable[pr.hand] = page;
                pr.useBit[pr.hand] = 1;  // 设置当前页面为访问过
                pageToIndex[page] = pr.hand;  // 更新映射
                pr.hand = (pr.hand + 1) % pr.numFrames;  // 移动时钟指针
                isPageFault = true;  // 标记为缺页
                break;
            } else {
                // 重置使用位并移动指针
                pr.useBit[pr.hand] = 0;
                pr.hand = (pr.hand + 1) % pr.numFrames;
            }
        }

        // 打印当前访问的页面信息、页表和标志位
        pr.displayPageTable(page, isPageFault, accessCount);  // 打印当前页表和标志位
    }

    // 访问完所有页面后，计算并显示缺页率
    std::cout << "总缺页率: " << pr.getPageFaultRate(sequence.size()) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(sequence.size()) << "%" << std::endl;
}

// 改进型Clock置换算法实现
void CLOCKpro(PageReplacement& pr, const std::vector<int>& sequence) {
    int accessCount = 0;
    std::unordered_map<int, int> pageToIndex; // 用于快速查找页面是否在页表中
    std::vector<int> modifyBit(pr.numFrames, 0); // 修改位，初始化为0
    pr.pageTable.assign(pr.numFrames, -1);  // 初始化页表，-1 表示无效页面

    for (int page : sequence) {
        ++accessCount;

        // 检查页面是否已经在页表中
        auto it = pageToIndex.find(page);
        if (it != pageToIndex.end()) {
            // 页面已存在，设置使用位
            pr.useBit[it->second] = 1;
            pr.displayPageTable(page, false, accessCount);
            continue;
        }

        // 页面不在页表中，发生缺页
        pr.pageFaults++;
        bool replaced = false;

        // 四轮扫描的置换逻辑
        for (int round = 1; !replaced && round <= 4; ++round) {
            for (int i = 0; i < pr.numFrames; ++i) {
                int index = (pr.hand + i) % pr.numFrames;

                // 根据当前轮次检查替换条件
                if ((round == 1 && pr.useBit[index] == 0 && modifyBit[index] == 0) ||
                    (round == 2 && pr.useBit[index] == 0 && modifyBit[index] == 1) ||
                    (round == 3 && pr.useBit[index] == 0 && modifyBit[index] == 0) ||
                    (round == 4 && pr.useBit[index] == 0 && modifyBit[index] == 1)) {

                    replaced = true;

                    // 如果当前帧有页面，执行置换
                    if (pr.pageTable[index] != -1) {
                        pageToIndex.erase(pr.pageTable[index]); // 从映射中移除被置换的页面
                        pr.pageReplacementCount++; // 更新置换计数
                    }

                    // 执行页面替换
                    pr.pageTable[index] = page;        // 新页面放入页表
                    pr.useBit[index] = 1;             // 设置使用位
                    modifyBit[index] = 0;             // 重置修改位
                    pageToIndex[page] = index;        // 更新映射
                    pr.hand = (index + 1) % pr.numFrames; // 移动时钟指针
                    break;
                }

                // 如果未替换，重置使用位（第二轮专用）
                if (round == 2) {
                    pr.useBit[index] = 0;
                }
            }
        }

        // 显示当前页表状态
        pr.displayPageTable(page, true, accessCount);
    }

    // 显示最终缺页率和置换率
    std::cout << "总缺页率: " << pr.getPageFaultRate(sequence.size()) << "%" << std::endl;
    std::cout << "总置换率: " << pr.getPageReplacementRate(sequence.size()) << "%" << std::endl;
}

// 生成页面访问序列
std::vector<int> generateSequence(int m, int e, int s, int length) {
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
                 "3. 最佳置换算法(OPT)\n"
                 "4. 简单Clock置换算法(Clock)\n"
                 "5. 改进型Clock置换算法(Clock Pro)\n";
    std::cout << "输入你的选择 (1/2/3/4/5): ";
    std::cin >> choice;

    PageReplacement pr(numFrames);
    switch (choice) {
        case 1:
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "\n<----------FIFO页面置换算法---------->\n";
            FIFO(pr, sequence);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "FIFO算法运行时间: " << duration.count() << " 秒\n";
            break;
        }
        case 2:
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "\n<----------LRU页面置换算法---------->\n";
            LRU(pr, sequence);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "LRU算法运行时间: " << duration.count() << " 秒\n";
            break;
        }
        case 3:
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "\n<----------OPT页面置换算法---------->\n";
            OPT(pr, sequence);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "OPT算法运行时间: " << duration.count() << " 秒\n";
            break;
        }
        case 4:
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "\n<----------Clock页面置换算法---------->\n";
            CLOCK(pr, sequence);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "Clock算法运行时间: " << duration.count() << " 秒\n";
            break;
        }
        case 5:
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "\n<----------Clock Pro页面置换算法---------->\n";
            CLOCKpro(pr, sequence);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "Clock Pro算法运行时间: " << duration.count() << " 秒\n";
            break;
        }
        default:
            std::cout << "无效选择。\n";
    }

    std::cout << "总缺页次数: " << pr.pageFaults << std::endl;
}

// 选择使用自定义序列或生成序列
std::vector<int> choosePageSequence() {
    int choice;
    std::cout << "请选择页面访问序列的生成方式:\n";
    std::cout << "1. 使用自定义序列\n";
    std::cout << "2. 使用生成函数生成序列\n";
    std::cout << "请输入选择 (1 或 2): ";
    std::cin >> choice;

    std::vector<int> sequence;

    if (choice == 1) {
        // 使用自定义页面访问序列
        sequence = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
        std::cout << "使用自定义页面访问序列: ";
        for (int page : sequence) {
            std::cout << page << " ";
        }
        std::cout << std::endl;
    } else if (choice == 2) {
        // 输入参数生成页面访问序列
        int m, e, s, length;
        std::cout << "请输入页面访问序列生成参数 (m, e, s, maxPage, length):\n";
        std::cout << "m（每次访问段的长度）: ";
        std::cin >> m;
        std::cout << "e（页面号范围）: ";
        std::cin >> e;
        std::cout << "s（段的数量）: ";
        std::cin >> s;
        std::cout << "length（生成序列的总长度）: ";
        std::cin >> length;

        // 调用生成页面访问序列的函数
        sequence = generateSequence(m, e, s, length);
    } else {
        std::cerr << "无效的选择！\n";
    }

    return sequence;
}