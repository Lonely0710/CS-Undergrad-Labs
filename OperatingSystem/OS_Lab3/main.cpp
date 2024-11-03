#include "CPUscheduling.h"

int main() {
    CPU_Scheduling scheduler;
    int numProcesses = 5;

    // 随机生成进程
    std::vector<Process> processes = scheduler.generateProcesses(numProcesses);
    scheduler.printInit(processes); // 输出生成的进程信息
    std::cout << std::endl;

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