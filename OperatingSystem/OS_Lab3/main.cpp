#include "CPUscheduling.h"

int main() {
    CPU_Scheduling scheduler;
    std::vector<Process> processes = {
            {1, 0, 8, 0, 0, 0, 8},
            {2, 1, 4, 0, 0, 0, 4},
            {3, 2, 9, 0, 0, 0, 9},
            {4, 3, 5, 0, 0, 0, 5}
    };
    // 定义测试进程列表orders：PID, AT, BT, WT, TT, FT
    // 其中 BT, WT, TT为0，用于存储算法计算后的结果

    int choice, timeQuantum;
    std::cout << "选择调度算法: 1. FCFS 2. SJF 3. HRRN 4. RR\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            scheduler.FCFS(processes);
            break;
        case 2:
            scheduler.SJF(processes);
            break;
        case 3:
            scheduler.HRRN(processes);
            break;
        case 4:
            std::cout << "输入时间片大小: ";
            std::cin >> timeQuantum;
            scheduler.RR(processes, timeQuantum);
            break;
        default:
            std::cout << "无效选择\n";
            return 1;
    }

    scheduler.printProcesses(processes);
    return 0;
}