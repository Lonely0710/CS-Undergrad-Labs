#include "pageReplacement.h"

int main() {
    int numFrames;  // 页框数量

    std::cout << "请输入页框数量: ";
    std::cin >> numFrames;

    std::vector<int> sequence = choosePageSequence();

    // 选择并运行页面置换算法
    selectAlgorithm(numFrames, sequence);

    return 0;
}
