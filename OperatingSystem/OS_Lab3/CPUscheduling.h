#ifndef CPUSCHEDULING_H
#define CPUSCHEDULING_H

#include <iostream>
#include <vector>
#include <algorithm>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime; // CPU时间
    int waitTime;
    int turnaroundTime; // 周转时间
    int finishTime;
    int remainingTime; // 用于RR调度
};

class CPU_Scheduling {
public:
    void FCFS(std::vector<Process>& processes);
    void SJF(std::vector<Process>& processes);
    void HRRN(std::vector<Process>& processes);
    void RR(std::vector<Process>& processes, int timeQuantum);
    void printProcesses(const std::vector<Process>& processes);
};


#endif //CPUSCHEDULING_H
