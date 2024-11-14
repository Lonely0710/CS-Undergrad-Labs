#include "pageReplacement.h"

int main() {
    int numFrames;  // 页框数量
    int m, e, s, maxPage, length;  // 序列生成的参数

    std::cout << "请输入页框数量: ";
    std::cin >> numFrames;

    std::cout << "请输入页面访问序列生成参数 (m, e, s, maxPage, length):\n";
    std::cout << "m（每次访问段的长度）: ";
    std::cin >> m;
    std::cout << "e（页面号范围）: ";
    std::cin >> e;
    std::cout << "s（段的数量）: ";
    std::cin >> s;
    std::cout << "maxPage（页面号的最大值）: ";
    std::cin >> maxPage;
    std::cout << "length（生成序列的总长度）: ";
    std::cin >> length;

    // 生成页面访问序列
    std::vector<int> sequence = generateSequence(m, e, s, maxPage, length);

    // 选择并运行页面置换算法
    selectAlgorithm(numFrames, sequence);

    return 0;
}